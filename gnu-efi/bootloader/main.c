#include <efi.h>
#include <efierr.h>
#include <efilib.h>
#include <elf.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define PSF1_FONT_MAGIC 0x0436

typedef struct {
    uint16_t Magic;
    uint8_t Mode;
    uint8_t CharSize;
} PSF1_Header;

typedef struct {
    PSF1_Header *Header;
    void *GlyphBuffer;
} PSF1_Font;

typedef struct {
    void *BaseAddress;
    size_t BufferSize;
    uint32_t Width,
        Height,
        PixelsPerScanLine;
} FrameBuffer;
FrameBuffer GlobalFrameBuffer;

typedef struct {
    EFI_MEMORY_DESCRIPTOR *MemoryMap;
    size_t MapSize,
        MapKey,
        DescriptorSize;
    uint32_t DescriptorVersion;
    size_t EntryCount;
} MemoryMapInfo;

typedef struct {
    FrameBuffer *GlobalFrameBuffer;
    PSF1_Font *PSF1_Font;
    MemoryMapInfo *MemoryMapInfo;
} BootInfo;

CHAR16 ErrMsgBuffer[64];

EFI_STATUS getMemoryMap(EFI_SYSTEM_TABLE *systemTable, MemoryMapInfo *memoryMapInfo) {
    memoryMapInfo->MapSize = 0;

    EFI_STATUS status = systemTable->BootServices->GetMemoryMap(
        &memoryMapInfo->MapSize,
        memoryMapInfo->MemoryMap,
        &memoryMapInfo->MapKey,
        &memoryMapInfo->DescriptorSize,
        &memoryMapInfo->DescriptorVersion);

    if (status == EFI_BUFFER_TOO_SMALL) {
        // ! Yes, hard coded value bad, I know. Will work something out.
        // TODO: no hard coded value
        // Fix -> loop?
        memoryMapInfo->MapSize += 0x100;

        EFI_STATUS s = systemTable->BootServices->AllocatePool(EfiLoaderData, memoryMapInfo->MapSize, (void **)&memoryMapInfo->MemoryMap);
        if (EFI_ERROR(s)) {
            StatusToString(ErrMsgBuffer, status);
            Print(L"[ERR]: Failed to allocate pool to hold MemoryMap. Reason: %s\n\r", ErrMsgBuffer);
            return s;
        }

        status = systemTable->BootServices->GetMemoryMap(
            &memoryMapInfo->MapSize,
            memoryMapInfo->MemoryMap,
            &memoryMapInfo->MapKey,
            &memoryMapInfo->DescriptorSize,
            &memoryMapInfo->DescriptorVersion);
        if (EFI_ERROR(status)) {
            StatusToString(ErrMsgBuffer, status);
            Print(L"[ERR]: Failed to get MemoryMap(1). Reason: %s\n\r", ErrMsgBuffer);
            return status;
        }
    } else {
        if (EFI_ERROR(status)) {
            StatusToString(ErrMsgBuffer, status);
            Print(L"[ERR]: Failed to get MemoryMap(0). Reason: %s\n\r", ErrMsgBuffer);
            return status;
        }
    }

    memoryMapInfo->EntryCount = memoryMapInfo->MapSize / memoryMapInfo->DescriptorSize;
    return EFI_SUCCESS;
}

EFI_STATUS initializeGOP() {
    EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;

    EFI_STATUS status = uefi_call_wrapper(BS->LocateProtocol, 3, &gopGuid, NULL, (void **)&gop);
    if (EFI_ERROR(status)) {
        StatusToString(ErrMsgBuffer, status);
        Print(L"[ERR]: Unable to locate GOP! Reason: %s\n\r", ErrMsgBuffer);
        return status;
    }
    Print(L"[OK]: GOP located.\n\r");

    Print(L"[INFO]: Querying output mode...\n\r");
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
    size_t infoSize, modeCount, nativeMode;

    status = uefi_call_wrapper(gop->QueryMode, 4, gop, (gop->Mode == NULL) ? 0 : gop->Mode->Mode, &infoSize, &info);  // Loving that identifier UEFI :/
    if (status == EFI_NOT_STARTED) {
        status = uefi_call_wrapper(gop->SetMode, 2, gop, 0);
    }

    if (EFI_ERROR(status)) {
        StatusToString(ErrMsgBuffer, status);
        Print(L"[ERR]: Unable to get GOP native mode! Reason: %s\n\r", ErrMsgBuffer);
        return status;
    } else {
        nativeMode = gop->Mode->Mode;
        modeCount = gop->Mode->MaxMode;
    }

    Print(L"\n\r| === GOP Available Video Modes === |\n\r");
    /* Query each mode */
    for (size_t i = 0; i < modeCount; i++) {
        status = uefi_call_wrapper(gop->QueryMode, 4, gop, i, &infoSize, &info);

        if (EFI_ERROR(status)) {
            Print(L"%03d: Failed to query.\n\r", i);
            continue;
        } else {
            short unsigned int *pxFormat;
            switch (info->PixelFormat) {
                case PixelRedGreenBlueReserved8BitPerColor: {
                    pxFormat = L"RGBResv-8bit";
                    break;
                }
                case PixelBlueGreenRedReserved8BitPerColor: {
                    pxFormat = L"BGRResv-8bit";
                    break;
                }
                case PixelBitMask: {
                    pxFormat = L"PixelBitmask";
                    break;
                }
                case PixelBltOnly: {
                    pxFormat = L"PixelBLTOnly";
                    break;
                }
                default:
                    pxFormat = L"Invalid";
            }

            Print(L"%03d(v%d): Res -> %dx%d, PixFormat -> %s, Pixels / ScanLine -> %d%s\n\r",
                  i,
                  info->Version,
                  info->HorizontalResolution,
                  info->VerticalResolution,
                  pxFormat,
                  info->PixelsPerScanLine,
                  (i == nativeMode) ? L"\t(Current)" : L"");
        }
    }

    status = uefi_call_wrapper(gop->SetMode, 2, gop, nativeMode);
    if (EFI_ERROR(status)) {
        StatusToString(ErrMsgBuffer, status);
        Print(L"[ERR]: Unable to set mode %03d. Reason: %s\n\r", nativeMode, ErrMsgBuffer);
    } else {
        Print(L"[OK]: Successfully set GOP mode: %03d\n\r", nativeMode);
    }

    GlobalFrameBuffer.BaseAddress = (void *)gop->Mode->FrameBufferBase;
    GlobalFrameBuffer.BufferSize = gop->Mode->FrameBufferSize;
    GlobalFrameBuffer.Width = gop->Mode->Info->HorizontalResolution;
    GlobalFrameBuffer.Height = gop->Mode->Info->VerticalResolution;
    GlobalFrameBuffer.PixelsPerScanLine = gop->Mode->Info->PixelsPerScanLine;

    return EFI_SUCCESS;
}

EFI_FILE *loadFile(EFI_FILE *directory, CHAR16 *path, EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable) {
    EFI_FILE *loadedFile;
    EFI_LOADED_IMAGE_PROTOCOL *loadedImage;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *fileSystem;

    systemTable->BootServices->HandleProtocol(imageHandle, &gEfiLoadedImageProtocolGuid, (void **)&loadedImage);
    systemTable->BootServices->HandleProtocol(loadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void **)&fileSystem);

    if (directory == NULL) {
        fileSystem->OpenVolume(fileSystem, &directory);
    }

    EFI_STATUS status = directory->Open(directory, &loadedFile, path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
    if (status != EFI_SUCCESS) {
        return NULL;
    }

    return loadedFile;
}

PSF1_Font *loadFont(EFI_FILE *directory, CHAR16 *path, EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable) {
    EFI_FILE *fontFile = loadFile(directory, path, imageHandle, systemTable);
    if (fontFile == NULL) {
        Print(L"[ERR]: Unable to load font file!\n\r");
        return NULL;
    }

    PSF1_Header *fontHeader;
    systemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSF1_Header), (void **)&fontHeader);

    size_t headerSize = sizeof(PSF1_Header);
    fontFile->Read(fontFile, &headerSize, fontHeader);

    if (fontHeader->Magic != PSF1_FONT_MAGIC) {
        Print(L"[ERR]: Font file had invalid magic/identifier bytes. Expected: 0x%x, Found: 0x%x\n\r",
              PSF1_FONT_MAGIC,
              fontHeader->Magic);
        return NULL;
    }

    size_t glyphBufferSize = fontHeader->CharSize * ((fontHeader->Mode == 1) ? 512 : 256);
    void *glyphBuffer;
    {
        fontFile->SetPosition(fontFile, sizeof(PSF1_Header));  // Move ahead of buffer
        systemTable->BootServices->AllocatePool(EfiLoaderData, glyphBufferSize, (void **)&glyphBuffer);
        fontFile->Read(fontFile, &glyphBufferSize, glyphBuffer);
    }

    PSF1_Font *font;
    systemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSF1_Font), (void **)&font);

    font->Header = fontHeader;
    font->GlyphBuffer = glyphBuffer;

    return font;
}

int memcmp(const void *ptr1, const void *ptr2, size_t n) {
    const unsigned char *a = ptr1, *b = ptr2;
    for (size_t i = 0; i < n; i++) {
        if (a[i] < b[i])
            return -1;
        else if (a[i] > b[i])
            return 1;
    }
    return 0;
}

EFI_STATUS efi_main(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable) {
    InitializeLib(imageHandle, systemTable);
    Print(L"[OK]: Hello from bootloader!\n\r");

    EFI_FILE *kernel = loadFile(NULL, L"kernel.elf", imageHandle, systemTable);
    if (kernel == NULL) {
        Print(L"[ERR]: Could not load kernel.\n\r");
        return EFI_LOAD_ERROR;
    } else {
        Print(L"[OK]: Kernel loaded successfully.\n\r");
    }

    Elf64_Ehdr elfHeader;
    {
        size_t fileInfoSize;
        EFI_FILE_INFO *fileInfo;
        kernel->GetInfo(kernel, &gEfiFileInfoGuid, &fileInfoSize, NULL);
        systemTable->BootServices->AllocatePool(EfiLoaderData, fileInfoSize, (void **)&fileInfo);
        kernel->GetInfo(kernel, &gEfiFileInfoGuid, &fileInfoSize, (void **)&fileInfo);

        size_t size = sizeof(elfHeader);
        kernel->Read(kernel, &size, &elfHeader);
    }

    if (memcmp(&elfHeader.e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0 ||
        elfHeader.e_ident[EI_CLASS] != ELFCLASS64 ||
        elfHeader.e_ident[EI_DATA] != ELFDATA2LSB ||
        elfHeader.e_type != ET_EXEC ||
        elfHeader.e_machine != EM_X86_64 ||
        elfHeader.e_version != EV_CURRENT) {
        Print(L"[ERR]: Kernel format is invalid!\n\r");
        return EFI_INVALID_PARAMETER;
    } else {
        Print(L"[OK]: Kernel format verified.\n\r");
    }

    Elf64_Phdr *programHeaders;
    {
        kernel->SetPosition(kernel, elfHeader.e_phoff);
        size_t size = elfHeader.e_phnum * elfHeader.e_phentsize;
        systemTable->BootServices->AllocatePool(EfiLoaderData, size, (void **)&programHeaders);
        kernel->Read(kernel, &size, programHeaders);
    }

    for (Elf64_Phdr *programHeader = programHeaders;
         (char *)programHeader < (char *)programHeaders + elfHeader.e_phnum * elfHeader.e_phentsize;
         programHeader = (Elf64_Phdr *)((char *)programHeader + elfHeader.e_phentsize)) {
        switch (programHeader->p_type) {
            case PT_LOAD: {
                int pages = (programHeader->p_memsz + 0x1000 - 1) / 0x1000;
                Elf64_Addr segment = programHeader->p_paddr;
                systemTable->BootServices->AllocatePages(AllocateAddress, EfiLoaderData, pages, &segment);

                kernel->SetPosition(kernel, programHeader->p_offset);
                size_t size = programHeader->p_filesz;
                kernel->Read(kernel, &size, (void *)segment);
                break;
            }
        }
    }
    Print(L"[OK]: Kernel program loaded.\n\r");

    Print(L"[INFO]: Accessing GOP...\n\r");
    EFI_STATUS status = initializeGOP();
    if (EFI_ERROR(status)) {
        StatusToString(ErrMsgBuffer, status);
        Print(L"[ERR]: %s\n\r", ErrMsgBuffer);

        return status;
    } else {
        Print(
            L"\n\r| === FrameBuffer Info === |\
			\n\rBaseAddress: 0x%x\
			\n\rBufferSize: 0x%x\
			\n\rWidth: %dpx\
			\n\rHeight: %dpx\
			\n\rPixels/ScanLine: %dpx\
			\n\r| ======================== |\
			\n\r\n\r",
            GlobalFrameBuffer.BaseAddress,
            GlobalFrameBuffer.BufferSize,
            GlobalFrameBuffer.Width,
            GlobalFrameBuffer.Height,
            GlobalFrameBuffer.PixelsPerScanLine);
    }

    Print(L"[INFO]: Loading PSF1 font: \"Terminus\"...\n\r");
    PSF1_Font *terminusFont = loadFont(NULL, L"Uni2-Terminus16.psf", imageHandle, systemTable);
    if (terminusFont == NULL) {
        return EFI_INVALID_PARAMETER;
    } else {
        Print(L"[OK]: Successfully loaded new font: Terminus[%d].\n\r",
              terminusFont->Header->CharSize);
    }

    Print(L"[INFO]: Accessing memory map...\n\r");
    MemoryMapInfo memoryMapInfo = {0, 0, 0, 0, 0, 0};
    status = getMemoryMap(systemTable, &memoryMapInfo);

    if (EFI_ERROR(status)) {
        return status;
    }

    /*
     * The MapKey is updated whenever the map is updated.
     * In order to ensure the key is valid, we must *NOT*
     * use any services that may update the map after
     * retrieval, thus invalidating the key.
     */
    status = systemTable->BootServices->ExitBootServices(imageHandle, memoryMapInfo.MapKey);
    if (EFI_ERROR(status)) {
        StatusToString(ErrMsgBuffer, status);

        Print(L"[ERR]: Failed to exit BootServices [%s]! Retrying...\n\r", ErrMsgBuffer);

        status = getMemoryMap(systemTable, &memoryMapInfo);
        if (EFI_ERROR(status)) {
            return status;
        }
        status = systemTable->BootServices->ExitBootServices(imageHandle, memoryMapInfo.MapKey);

        if (EFI_ERROR(status)) {
            StatusToString(ErrMsgBuffer, status);
            Print(L"[FATAL]: Failed to exit BootServices again [%s]! Giving up...", ErrMsgBuffer);
            return status;
        }
    }

    void (*kernelStart)(BootInfo) = ((__attribute__((sysv_abi)) void (*)(BootInfo))elfHeader.e_entry);
    kernelStart((BootInfo){&GlobalFrameBuffer, terminusFont, &memoryMapInfo});

    return EFI_SUCCESS;  // Exit the UEFI application
}

#include "efi/EFIMemory.hpp"

#include "memory/Memory.hpp"

const char* EFI_MEMORY_TYPE_STRINGS[] = {
    "EfiReservedMemoryType",
    "EfiLoaderCode",
    "EfiLoaderData",
    "EfiBootServicesCode",
    "EfiBootServicesData",
    "EfiRuntimeServicesCode",
    "EfiRuntimeServicesData",
    "EfiConventionalMemory",
    "EfiUnusableMemory",
    "EfiACPIReclaimMemory",
    "EfiACPIMemoryNVS",
    "EfiMemoryMappedIO",
    "EfiMemoryMappedIOPortSpace",
    "EfiPalCode",
    "EfiPersistentMemory",
    "EfiUnacceptedMemoryType"};

void printEFIMemoryMapInfo(MemoryMapInfo* memMapInfo, TextRenderer* OUTPUT) {
    OUTPUT->_print(
        "\n\r+ =================== + =================== + =================== + =================== + =================== +\n\r\
|                     |      DESC ADDR      |      DESC SIZE      |   MEMORY MAP SIZE   |   MAP ENTRY COUNT   |\n\r\
|   MEMORY MAP INFO   I ------------------- I ------------------- I ------------------- I ------------------- |\n\r\
|                     |  0x");
    OUTPUT->_print(to_hstring((uint64_t)memMapInfo->MemoryMap));
    OUTPUT->_print(" |  0x");
    OUTPUT->_print(to_hstring((uint64_t)memMapInfo->DescriptorSize));
    OUTPUT->_print(" |  0x");
    OUTPUT->_print(to_hstring((uint64_t)memMapInfo->MapSize));
    OUTPUT->_print(" | ");
    OUTPUT->_print(str_padleft(to_string((uint64_t)memMapInfo->EntryCount), 19, ' '));
    OUTPUT->_print(" |\n\r");
    OUTPUT->_print("+ =================== + =================== + =================== + =================== + =================== +\n\r");
    OUTPUT->_print("| ENTRIES:            |");
    OUTPUT->_print(str_padleft("|\n\r", 111 - 21, ' '));
    OUTPUT->_print("+ ------------------- +");
    OUTPUT->_print(str_padleft("|\n\r", 111 - 21, ' '));

    size_t memSizes[EfiMaxMemoryType] = {};
    for (size_t i = 0; i < memMapInfo->EntryCount; i++) {
        EFI_MEMORY_DESCRIPTOR desc = *(EFI_MEMORY_DESCRIPTOR*)((uint64_t)memMapInfo->MemoryMap + (i * memMapInfo->DescriptorSize));
        memSizes[desc.Type] += desc.PageCount * PAGE_SIZE;
    }

    /* Render 3 entries on each row */
    size_t printedEntries = 0;
    bool newLineDue = false;
    for (size_t i = 0; i < EfiMaxMemoryType; i++) {
        if (newLineDue) {
            OUTPUT->_print(str_padleft("|\n\r", 111 - OUTPUT->CursorPosition.X + 2, ' '));
            OUTPUT->_print("| ");
            newLineDue = false;
        } else if (printedEntries == 0) {
            OUTPUT->_print("| ");
        }

        if (memSizes[i]) {
            printedEntries++;

            OUTPUT->_print(EFI_MEMORY_TYPE_STRINGS[i]);
            OUTPUT->_print("->");
            OUTPUT->_print(to_string((double)memSizes[i] / 1000, 1));
            OUTPUT->_print(
                ((printedEntries != 0) && (printedEntries % 3) == 0)
                    ? str_padright("KB,", 110 - OUTPUT->CursorPosition.X, ' ')
                    : "KB, ");

            if ((printedEntries % 3) == 0) {
                newLineDue = true;
            }
        }
    }
    OUTPUT->_print(str_padleft("|\n\r", 111 - OUTPUT->CursorPosition.X + 2, ' '));

    OUTPUT->_print("+ ");
    OUTPUT->_print(str_padleft(" +\n\r", 111, '='));
}

#include <stddef.h>
#include <stdint.h>

#include "FrameBuffer.hpp"
#include "Math.hpp"
#include "Str.hpp"
#include "efi/EFIMemory.hpp"
#include "font/PSF1.hpp"
#include "memory/Memory.hpp"
#include "memory/PageFrameAllocator.hpp"
#include "utils/BitMap.hpp"

struct BootInfo {
    FrameBuffer *GlobalFrameBuffer;
    PSF1_Font *LoadedFont;
    MemoryMapInfo *MemMapInfo;
};
FrameBuffer g_FrameBuffer;

extern size_t _KernelBeginAddr, _KernelEndAddr;

void paintDeskflop(uint32_t colorByte) {
    for (uint32_t y = 0; y < g_FrameBuffer.Height; y++) {
        for (uint32_t x = 0; x < g_FrameBuffer.PixelsPerScanLine; x++) {
            *(uint32_t *)(x + (y * g_FrameBuffer.PixelsPerScanLine) + (uint32_t *)g_FrameBuffer.BaseAddress) = colorByte;
        }
    }
}

extern "C" void _kernelEntry(BootInfo bootInfo) {
    g_FrameBuffer = *bootInfo.GlobalFrameBuffer;
    TextRenderer OUTPUT = TextRenderer(g_FrameBuffer, *bootInfo.LoadedFont, 0xffffffff);
    MemoryMapInfo memMapInfo = *bootInfo.MemMapInfo;

    paintDeskflop(0x0000000);
    /* paintDeskflop(0xdeadbeef); */

    OUTPUT._print("[INFO]: Hello from the kernel! :D\n\r");

    printEFIMemoryMapInfo(&OUTPUT, &memMapInfo);

    PageFrameAllocator pageFrameAllocator;
    pageFrameAllocator.readEFIMemoryMap(&memMapInfo);

    /* Reserve pages of kernel just in case */
    size_t kernelSize = ((&_KernelEndAddr - &_KernelBeginAddr) / PAGE_SIZE) + 1;
    pageFrameAllocator.lockPages((void *)&_KernelBeginAddr, kernelSize);

    OUTPUT._print("\n\rTotal Memory Size: ");
    OUTPUT._print(to_string(getTotalMemorySize(&memMapInfo) / 0x400));
    OUTPUT._print("KiB\n\r");
    OUTPUT._print("Free Memory Size: ");
    OUTPUT._print(to_string(pageFrameAllocator.getFreeMemorySize() / 0x400));
    OUTPUT._print("KiB\n\r");
    OUTPUT._print("Used Memory Size: ");
    OUTPUT._print(to_string(pageFrameAllocator.getUsedMemorySize() / 0x400));
    OUTPUT._print("KiB\n\r");
    OUTPUT._print("Reserved Memory Size: ");
    OUTPUT._print(to_string(pageFrameAllocator.getReservedMemorySize() / 0x400));
    OUTPUT._print("KiB\n\r");

    for (size_t i = 0; i < 32; i++) {
        OUTPUT._print(to_hstring((uint64_t)pageFrameAllocator.requestPage()));
        OUTPUT._print("\n\r");
    }

    while (true) {
    }
}

#include "utils/KernelUtility.hpp"

extern "C" void _kernelEntry(BootInfo bootInfo) {
    TextRenderer OUTPUT = TextRenderer(*bootInfo.GlobalFrameBuffer, *bootInfo.LoadedFont, 0xffffffff);
    MemoryMapInfo memMapInfo = *bootInfo.MemMapInfo;

    initializeKernel(&bootInfo);

    OUTPUT._print("[INFO]: Hello from the kernel! :D\n\r");
    OUTPUT._print(to_string(g_PageFrameAllocator.getFreeMemorySize()));
    OUTPUT._print("\n\r");
    OUTPUT._print(to_string(g_PageFrameAllocator.getUsedMemorySize()));
    OUTPUT._print("\n\r");
    OUTPUT._print(to_string(g_PageFrameAllocator.getReservedMemorySize()));
    OUTPUT._print("\n\r");

    printEFIMemoryMapInfo(&OUTPUT, &memMapInfo);

    while (true) {
    }
}

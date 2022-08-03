#include "utils/KernelUtility.hpp"

extern "C" void _kernelEntry(BootInfo bootInfo) {
    MemoryMapInfo memMapInfo = *bootInfo.MemMapInfo;

    initializeKernel(&bootInfo);
    size_t totalMemSize = getTotalMemorySize();

    g_TextRenderer._print("[INFO]: Hello from the kernel! :D\n\r");

    g_TextRenderer._print("\n\r");
    g_TextRenderer._print(str_padright("Free RAM Size: ", 20, ' '));
    g_TextRenderer._print(str_padleft(to_string((double)g_PageFrameAllocator.getFreeMemorySize() / 0x400, 3), 16, ' '));
    g_TextRenderer._print("MiB\n\r");
    g_TextRenderer._print(str_padright("Used RAM Size: ", 20, ' '));
    g_TextRenderer._print(str_padleft(to_string((double)g_PageFrameAllocator.getUsedMemorySize() / 0x400, 3), 16, ' '));
    g_TextRenderer._print("MiB\n\r");
    g_TextRenderer._print(str_padright("Reserved RAM Size: ", 20, ' '));
    g_TextRenderer._print(str_padleft(to_string((double)g_PageFrameAllocator.getReservedMemorySize() / 0x400, 3), 16, ' '));
    g_TextRenderer._print("MiB\n\r");
    g_TextRenderer._print(str_padright("Total RAM Size: ", 20, ' '));
    g_TextRenderer._print(str_padleft(to_string((double)totalMemSize / 0x100000, 3), 16, ' '));
    g_TextRenderer._print("MiB\n\r");

    asm("int $0x0e");

    while (true) {
    }
}

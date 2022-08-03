#include "utils/KernelUtility.hpp"

void prepareMemory(BootInfo *bootInfo) {
    g_PageFrameAllocator.readEFIMemoryMap(bootInfo->MemMapInfo);
    size_t totalMemSize = getTotalMemorySize(bootInfo->MemMapInfo);

    /* Reserve pages of kernel just in case */
    size_t kernelSize = ((&_KernelEnd - &_KernelBegin) / PAGE_SIZE) + 1;
    g_PageFrameAllocator.lockPages((void *)&_KernelBegin, kernelSize);

    PageTable *pageMapLevel4 = (PageTable *)g_PageFrameAllocator.requestPage();
    memset(pageMapLevel4, 0, PAGE_SIZE);

    PageTableManager pageTableManager(pageMapLevel4);
    for (size_t i = 0; i < totalMemSize; i += PAGE_SIZE) {
        pageTableManager.mapMemory((void *)i, (void *)i);
    }

    uint64_t frameBuffBase = (uint64_t)bootInfo->GlobalFrameBuffer->BaseAddress;
    uint64_t frameBuffSize = (uint64_t)bootInfo->GlobalFrameBuffer->BufferSize + PAGE_SIZE;
    g_PageFrameAllocator.lockPages((void *)frameBuffBase, frameBuffSize / PAGE_SIZE + 1);
    for (size_t i = frameBuffBase; i < frameBuffBase + frameBuffSize; i += PAGE_SIZE) {
        pageTableManager.mapMemory((void *)i, (void *)i);
    }

    asm("mov %0, %%cr3"
        :
        : "r"(pageMapLevel4));

    memset(bootInfo->GlobalFrameBuffer->BaseAddress, 0, bootInfo->GlobalFrameBuffer->BufferSize);
}

void initializeKernel(BootInfo *bootInfo) {
    prepareMemory(bootInfo);
}

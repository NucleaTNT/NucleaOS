#include "memory/PageFrameAllocator.hpp"

#include <stddef.h>
#include <stdint.h>

static uint64_t g_FreeMemorySize = 0,
                g_UsedMemorySize = 0,
                g_ReservedMemorySize = 0;
static bool g_IsInitialized = false;

void PageFrameAllocator::readEFIMemoryMap(MemoryMapInfo *memMapInfo) {
    if (g_IsInitialized) {
        return;
    }
    g_IsInitialized = true;

    /*
     * Iterate through all memory segments to find the largest to store the
     * PageBitMap in.
     */
    EFI_MEMORY_DESCRIPTOR *largestMemorySegment;
    for (size_t i = 0; i < memMapInfo->EntryCount; i++) {
        EFI_MEMORY_DESCRIPTOR *desc = (EFI_MEMORY_DESCRIPTOR *)((uint64_t)memMapInfo->MemoryMap + (i * memMapInfo->DescriptorSize));
        if (desc->Type == EfiConventionalMemory) {
            if ((largestMemorySegment == nullptr) || (desc->PageCount > largestMemorySegment->PageCount)) {
                largestMemorySegment = desc;
            }
        }
    }

    size_t totalMemorySize = getTotalMemorySize(memMapInfo);
    g_FreeMemorySize = totalMemorySize;

    /* Place the PageBitMap in the largest available memory segment. */
    uint8_t *pageBitMapBuffer = (uint8_t *)largestMemorySegment->PhysicalAddress;
    size_t pageBitMapSize = ((totalMemorySize / PAGE_SIZE) / 8) + 1;
    _pageBitMap = BitMap(pageBitMapBuffer, pageBitMapSize);

    /*
     * Default to all memory being reserved and then only freeing memory that
     * is safe to use (EfiConventionalMemory).
     */
    reservePages(0, (totalMemorySize / PAGE_SIZE) + 1);
    for (size_t i = 0; i < memMapInfo->EntryCount; i++) {
        EFI_MEMORY_DESCRIPTOR *desc = (EFI_MEMORY_DESCRIPTOR *)((uint64_t)memMapInfo->MemoryMap + (i * memMapInfo->DescriptorSize));
        if (desc->Type == EfiConventionalMemory) {
            releasePages(desc->PhysicalAddress, desc->PageCount);
        }
    }

    /* Reserve pages of PageBitMap. */
    reservePages(pageBitMapBuffer, pageBitMapSize / PAGE_SIZE + 1);

    /* Reserve all pages between 0x000000 and 0x100000 (Safety) */
    reservePages(0, 0x100);
}

void PageFrameAllocator::freePages(void *address, size_t count) {
    for (size_t i = 0; i < count; i++) {
        uint64_t index = ((uint64_t)address / PAGE_SIZE) + i;
        if (!_pageBitMap[index]) {
            continue;
        }
        _pageBitMap.Set(index, false);
        g_FreeMemorySize += PAGE_SIZE;
        g_UsedMemorySize -= PAGE_SIZE;
    }
}

void PageFrameAllocator::lockPages(void *address, size_t count) {
    for (size_t i = 0; i < count; i++) {
        uint64_t index = ((uint64_t)address / PAGE_SIZE) + i;
        if (_pageBitMap[index]) {
            continue;
        }
        _pageBitMap.Set(index, true);
        g_FreeMemorySize -= PAGE_SIZE;
        g_UsedMemorySize += PAGE_SIZE;
    }
}
void PageFrameAllocator::reservePages(void *address, size_t count) {
    for (size_t i = 0; i < count; i++) {
        uint64_t index = ((uint64_t)address / PAGE_SIZE) + i;
        if (_pageBitMap[index]) {
            continue;
        }
        _pageBitMap.Set(index, true);
        g_FreeMemorySize -= PAGE_SIZE;
        g_ReservedMemorySize += PAGE_SIZE;
    }
}

void PageFrameAllocator::releasePages(void *address, size_t count) {
    for (size_t i = 0; i < count; i++) {
        uint64_t index = ((uint64_t)address / PAGE_SIZE) + i;
        if (!_pageBitMap[index]) {
            continue;
        }
        _pageBitMap.Set(index, false);
        g_FreeMemorySize += PAGE_SIZE;
        g_ReservedMemorySize -= PAGE_SIZE;
    }
}

void *PageFrameAllocator::requestPage() {
    for (size_t i = 0; i < _pageBitMap.getSize() * 8; i++) {
        if (_pageBitMap[i]) {
            continue;
        }
        lockPages((void *)(i * PAGE_SIZE));
        return (void *)(i * PAGE_SIZE);
    }

    // We've run out of free pages.
    // TODO: Perform hard disk swap.
    return nullptr;
}

uint64_t PageFrameAllocator::getFreeMemorySize() {
    if (g_IsInitialized) {
        return g_FreeMemorySize;
    }
    return 0;
}

uint64_t PageFrameAllocator::getUsedMemorySize() {
    if (g_IsInitialized) {
        return g_UsedMemorySize;
    }
    return 0;
}

uint64_t PageFrameAllocator::getReservedMemorySize() {
    if (g_IsInitialized) {
        return g_ReservedMemorySize;
    }
    return 0;
}
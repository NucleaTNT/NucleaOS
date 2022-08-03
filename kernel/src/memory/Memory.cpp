#include "memory/Memory.hpp"

#include "efi/EFIMemory.hpp"

size_t getTotalMemorySize(MemoryMapInfo *memMapInfo) {
    static size_t totalMemorySize = 0;

    if (totalMemorySize <= 0 && (memMapInfo != nullptr)) {
        for (size_t i = 0; i < memMapInfo->EntryCount; i++) {
            EFI_MEMORY_DESCRIPTOR desc = *(EFI_MEMORY_DESCRIPTOR *)((uint64_t)memMapInfo->MemoryMap + (i * memMapInfo->DescriptorSize));
            totalMemorySize += desc.PageCount * PAGE_SIZE;
        }
    }

    return totalMemorySize;
}

void *memcpy(void *dest, const void *src, size_t count) {
    const uint8_t *s = (const uint8_t *)src;
    uint8_t *d = (uint8_t *)dest;

    while (count--) {
        *d++ = *s++;
    }

    return dest;
}

void *memset(void *dest, uint8_t val, size_t count) {
    uint8_t *d = (uint8_t *)dest;

    while (count--) {
        *d++ = val;
    }

    return dest;
}
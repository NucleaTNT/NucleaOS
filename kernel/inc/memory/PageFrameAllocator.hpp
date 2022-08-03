#pragma once

#include <stddef.h>

#include "efi/EFIMemory.hpp"
#include "memory/Memory.hpp"
#include "utils/BitMap.hpp"

/**
 * @brief Used to allocate and deallocate pages for use, storing each page's
 * state in the private BitMap member _pageBitMap.
 */
class PageFrameAllocator {
   private:
    /**
     * @brief BitMap containing the availability of each page in RAM.
     * Set -> Used/Reserved, Unset -> Free
     */
    BitMap _pageBitMap = BitMap(nullptr, 0);

    /**
     * @brief Reserves free pages so they cannot be allocated for use.
     *
     * @param address Address to start reserving pages from.
     * @param count Number of pages to reserve.
     */
    void reservePages(void *address, size_t count = 1);

    /**
     * @brief Releases reserved pages so they can be allocated for use.
     *
     * @param address Address to start releasing pages from.
     * @param count Number of pages to release.
     */
    void releasePages(void *address, size_t count = 1);

   public:
    /**
     * @brief
     *
     * @param address Address to start freeing pages from.
     * @param count Number of pages to free.
     */
    void freePages(void *address, size_t count = 1);

    /**
     * @brief
     *
     * @param address Address to start locking pages from.
     * @param count Number of pages to lock.
     */
    void lockPages(void *address, size_t count = 1);

    /**
     * @brief Returns a page for use.
     *
     * @return Pointer to allocated page.
     */
    void *requestPage();

    /**
     * @brief
     *
     * @param memMapInfo Pointer to the MemoryMapInfo to read from.
     */
    void readEFIMemoryMap(MemoryMapInfo *memMapInfo);

    /**
     * @return Current size of free RAM in bytes.
     */
    size_t getFreeMemorySize();

    /**
     * @return Current size of used RAM in bytes.
     */
    size_t getUsedMemorySize();

    /**
     * @return Current size of reserved RAM in bytes.
     */
    size_t getReservedMemorySize();
};

extern PageFrameAllocator g_PageFrameAllocator;
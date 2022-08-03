#include "memory/PageTableManager.hpp"

#include <stdint.h>

#include "memory/Memory.hpp"
#include "memory/PageFrameAllocator.hpp"
#include "memory/Paging.hpp"

PageTableManager::PageTableManager(PageTable* pageMapLevel4Address) {
    _pageMapLevel4 = pageMapLevel4Address;
}

/*
 * Walks through the levels of the PageMap to set the appropriate PageEntry.
 * This one is a bit of a maze, even I'm not too sure.
 */
void PageTableManager::mapMemory(void* virtualAddress, void* physicalAddress) {
    PageMapIndex virtMemIndex((uint64_t)virtualAddress);
    PageDirectoryEntry currentEntry;

    currentEntry = _pageMapLevel4->DirectoryEntries[virtMemIndex.PageDirectoryPointerIndex];
    PageTable* pageDirectoryPointer;
    if (!currentEntry.getFlag(PRESENT)) {
        pageDirectoryPointer = (PageTable*)g_PageFrameAllocator.requestPage();
        memset(pageDirectoryPointer, 0, PAGE_SIZE);

        currentEntry.setAddress((uint64_t)pageDirectoryPointer >> 12);
        currentEntry.setFlag(PRESENT, true);
        currentEntry.setFlag(READ_WRITE, true);
        _pageMapLevel4->DirectoryEntries[virtMemIndex.PageDirectoryPointerIndex] = currentEntry;
    } else {
        pageDirectoryPointer = (PageTable*)((uint64_t)currentEntry.getAddress() << 12);
    }

    currentEntry = pageDirectoryPointer->DirectoryEntries[virtMemIndex.PageDirectoryIndex];
    PageTable* pageDirectory;
    if (!currentEntry.getFlag(PRESENT)) {
        pageDirectory = (PageTable*)g_PageFrameAllocator.requestPage();
        memset(pageDirectory, 0, PAGE_SIZE);

        currentEntry.setAddress((uint64_t)pageDirectory >> 12);
        currentEntry.setFlag(PRESENT, true);
        currentEntry.setFlag(READ_WRITE, true);
        pageDirectoryPointer->DirectoryEntries[virtMemIndex.PageDirectoryIndex] = currentEntry;
    } else {
        pageDirectory = (PageTable*)((uint64_t)currentEntry.getAddress() << 12);
    }

    currentEntry = pageDirectory->DirectoryEntries[virtMemIndex.PageTableIndex];
    PageTable* pageTable;
    if (!currentEntry.getFlag(PRESENT)) {
        pageTable = (PageTable*)g_PageFrameAllocator.requestPage();
        memset(pageTable, 0, PAGE_SIZE);

        currentEntry.setAddress((uint64_t)pageTable >> 12);
        currentEntry.setFlag(PRESENT, true);
        currentEntry.setFlag(READ_WRITE, true);
        pageDirectory->DirectoryEntries[virtMemIndex.PageTableIndex] = currentEntry;
    } else {
        pageTable = (PageTable*)((uint64_t)currentEntry.getAddress() << 12);
    }

    currentEntry = pageTable->DirectoryEntries[virtMemIndex.PageEntryIndex];
    currentEntry.setAddress((uint64_t)physicalAddress >> 12);
    currentEntry.setFlag(PRESENT, true);
    currentEntry.setFlag(READ_WRITE, true);
    pageTable->DirectoryEntries[virtMemIndex.PageEntryIndex] = currentEntry;
}
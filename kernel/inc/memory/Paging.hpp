#pragma once
#include <stdint.h>

enum PageTableFlag {
    PRESENT,
    READ_WRITE,
    USER_SUPER,
    WRITE_THROUGH,
    CACHE_DISABLED,
    ACCESSED,
    LARGE_PAGES = 7,
    AVAILABLE_BIT_0 = 9,
    AVAILABLE_BIT_1,
    AVAILABLE_BIT_2,
    NO_EXECUTE = 63  // Only supported on some systems
};

struct PageDirectoryEntry {
    uint64_t Value;

    uint64_t getAddress();
    void setAddress(uint64_t address);

    bool getFlag(PageTableFlag flag);
    void setFlag(PageTableFlag flag, bool state);
};

struct PageTable {
    PageDirectoryEntry DirectoryEntries[512];
} __attribute__((aligned(0x1000)));

struct PageMapIndex {
    uint64_t PageDirectoryPointerIndex;  // Level 4
    uint64_t PageDirectoryIndex;         // Level 3
    uint64_t PageTableIndex;             // Level 2
    uint64_t PageEntryIndex;             // Level 1

    PageMapIndex(uint64_t virtualAddress);
};

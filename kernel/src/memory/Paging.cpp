#include "memory/Paging.hpp"

PageMapIndex::PageMapIndex(uint64_t virtualAddress) {
    virtualAddress >>= 12;
    PageEntryIndex = virtualAddress & 0x1ff;

    virtualAddress >>= 9;
    PageTableIndex = virtualAddress & 0x1ff;

    virtualAddress >>= 9;
    PageDirectoryIndex = virtualAddress & 0x1ff;

    virtualAddress >>= 9;
    PageDirectoryPointerIndex = virtualAddress & 0x1ff;
}

uint64_t PageDirectoryEntry::getAddress() {
    return ((Value & 0x000ffffffffff000) >> 12);
}

void PageDirectoryEntry::setAddress(uint64_t address) {
    address &= 0x000000ffffffffff;
    Value &= 0xfff0000000000fff;
    Value |= (address << 12);
}

bool PageDirectoryEntry::getFlag(PageTableFlag flag) {
    return (Value & (1 << flag));
}

void PageDirectoryEntry::setFlag(PageTableFlag flag, bool state) {
    uint64_t bitMask = (uint64_t)1 << flag;

    Value &= ~bitMask;
    if (state) {
        Value |= bitMask;
    }
}
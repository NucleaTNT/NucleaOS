#pragma once
#include "memory/Paging.hpp"

class PageTableManager {
   private:
    PageTable* _pageMapLevel4;

   public:
    PageTableManager(PageTable* pageMapLevel4Address);

    void mapMemory(void* virtualAddress, void* physicalAddress);
};

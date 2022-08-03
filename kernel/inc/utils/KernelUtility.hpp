#pragma once

#include <stddef.h>
#include <stdint.h>

#include "FrameBuffer.hpp"
#include "Math.hpp"
#include "Str.hpp"
#include "efi/EFIMemory.hpp"
#include "font/PSF1.hpp"
#include "memory/Memory.hpp"
#include "memory/PageFrameAllocator.hpp"
#include "memory/PageTableManager.hpp"
#include "memory/Paging.hpp"
#include "utils/BitMap.hpp"

struct BootInfo {
    FrameBuffer *GlobalFrameBuffer;
    PSF1_Font *LoadedFont;
    MemoryMapInfo *MemMapInfo;
};

extern size_t _KernelBegin, _KernelEnd;

void initializeKernel(BootInfo *BootInfo);
#pragma once

#include <stddef.h>
#include <stdint.h>

#include "efi/EFIMemory.hpp"
#include "font/PSF1.hpp"
#include "io/graphics/FrameBuffer.hpp"
#include "io/mouse/PS2_Mouse.hpp"
#include "memory/Memory.hpp"
#include "memory/PageFrameAllocator.hpp"
#include "memory/PageTableManager.hpp"
#include "memory/Paging.hpp"
#include "utils/BitMap.hpp"
#include "utils/Math.hpp"
#include "utils/Str.hpp"

struct BootInfo {
    FrameBuffer *GlobalFrameBuffer;
    PSF1_Font *LoadedFont;
    MemoryMapInfo *MemMapInfo;
};

extern size_t _KernelBegin, _KernelEnd;

void initializeKernel(BootInfo *BootInfo);
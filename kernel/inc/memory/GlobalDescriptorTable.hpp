#pragma once
#include <stdint.h>

#include "memory/Memory.hpp"

struct GDTDescriptor {
    uint16_t Size;
    uint64_t Offset;
} __attribute__((packed));

struct GDTEntry {
    uint16_t Limit_0;
    uint16_t Base_0;
    uint8_t Base_1;
    uint8_t AccessByte;
    uint8_t Limit_1_Flags;  // Bits[0..3] -> Upper Limit Bits, Bits[4..7] -> Flags
    uint8_t Base_2;
} __attribute__((packed));

struct GlobalDescriptorTable {
    GDTEntry KernelNull;  // Selector: 0x00
    GDTEntry KernelCode;  // Selector: 0x08
    GDTEntry KernelData;  // Selector: 0x10
    GDTEntry UserNull;    // Selector: 0x18
    GDTEntry UserCode;    // Selector: 0x20
    GDTEntry UserData;    // Selector: 0x28
} __attribute__((packed)) __attribute__((aligned(PAGE_SIZE)));

extern GlobalDescriptorTable g_GlobalDescriptorTable;
extern "C" void _loadGDT(GDTDescriptor *gdtDescriptor);
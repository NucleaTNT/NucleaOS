#pragma once

#include <stddef.h>
#include <stdint.h>

#include "io/graphics/TextRenderer.hpp"
#include "utils/Str.hpp"

/**
 * @brief Enum containing all the possible types of memory segments in the
 * EFIMemoryMap.
 */
enum EFI_MEMORY_TYPE {
    EfiReservedMemoryType,
    EfiLoaderCode,
    EfiLoaderData,
    EfiBootServicesCode,
    EfiBootServicesData,
    EfiRuntimeServicesCode,
    EfiRuntimeServicesData,
    EfiConventionalMemory,
    EfiUnusableMemory,
    EfiACPIReclaimMemory,
    EfiACPIMemoryNVS,
    EfiMemoryMappedIO,
    EfiMemoryMappedIOPortSpace,
    EfiPalCode,
    EfiPersistentMemory,
    EfiUnacceptedMemoryType,
    EfiMaxMemoryType
};

/**
 * @brief Struct representing the EFI Memory Descriptor's stored in the
 * EFI MemoryMap.
 */
struct EFI_MEMORY_DESCRIPTOR {
    EFI_MEMORY_TYPE Type;       // Type of the descriptor. Ranges from [0..EfiMaxMemoryType].
    uint64_t* PhysicalAddress;  // Physical address of the memory this descriptor describes.
    uint64_t* VirtualAddress;   // Virtual address of the memory this descriptor describes.
    uint64_t PageCount;         // Number of pages that the memory this descriptor describes occupies.
    uint64_t Attributes;        // Attributes relating to the memory this descriptor describes.
};

struct MemoryMapInfo {
    EFI_MEMORY_DESCRIPTOR* MemoryMap;  // Pointer to the first EFI_MEMORY_DESCRIPTOR in the EFIMemoryMap.
    size_t MapSize,                    // Size of the EFIMemoryMap in bytes.
        MapKey,                        // Key provided by the firmware relating to the current EFIMemoryMap.
        DescriptorSize;                // Size of an EFI_MEMORY_DESCRIPTOR in bytes.
    uint32_t DescriptorVersion;        // Version of the EFI_MEMORY_DESCRIPTORs used by the current EFIMemoryMap.
    size_t EntryCount;                 // Number of entries in the current EFIMemoryMap.
};

extern const char* EFI_MEMORY_TYPE_STRINGS[];

/**
 * @brief Pretty little method to display the EFIMemoryMap to the screen.
 *
 * @param memMapInfo Pointer to the MemoryMapInfo to read from.
 * @param OUTPUT The TextRenderer to output the generated table to.
 */
void printEFIMemoryMapInfo(MemoryMapInfo* memMapInfo, TextRenderer* OUTPUT = &g_TextRenderer);

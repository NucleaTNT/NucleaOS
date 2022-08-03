#pragma once

#include <stddef.h>
#include <stdint.h>

#include "efi/EFIMemory.hpp"

// Size of a single page in memory.
#define PAGE_SIZE 0x1000

/**
 * @brief Gets the total size of RAM in bytes.
 *
 * Calculates the total size of RAM on the system by iterating through each
 * EFI_MEMORY_DESCRIPTOR in the provided memMapInfo's MemoryMap. This sum
 * includes memory types that are NOT available for use by the system such as
 * EfiMemoryReserved.
 *
 * @param[in] memMapInfo Pointer to MemoryMapInfo struct to read from.
 *
 * @return Total size of RAM in bytes.
 */
size_t getTotalMemorySize(MemoryMapInfo *memMapInfo = nullptr);

/**
 * @brief Copies n(count) values found at src to dest.
 *
 * Both pointers are treated as uint8_t*'s when copying values.
 *
 * @param dest Pointer to the destination address to copy the values at src to.
 * @param src Pointer to the address to copy values from to the dest address.
 * @param count Number of values to copy before exiting.
 * @return Pointer to the destinination address provided as a parameter.
 */
void *memcpy(void *dest, const void *src, size_t count);

/**
 * @brief Sets n(count) bytes to val, starting from address dest.
 *
 * The dest pointer is treated as a uint8_t* when setting the bytes.
 *
 * @param dest Pointer to the destination address to begin setting bytes to val from.
 * @param val Value to set n(count) bytes to, starting from dest.
 * @param count Number of bytes to set before exiting.
 * @return Pointer to the destinination address provided as a parameter.
 */
void *memset(void *dest, uint8_t val, size_t count);
#pragma once

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Struct to represent the FrameBuffer used by the Graphics Output
 * Protocol (GOP).
 */
struct FrameBuffer {
    void *BaseAddress;           // Pointer to the base address of the FrameBuffer.
    size_t BufferSize;           // Size of the FrameBuffer in bytes.
    uint32_t Width;              // Width of the FrameBuffer in pixels.
    uint32_t Height;             // Height of the FrameBuffer in pixels.
    uint32_t PixelsPerScanLine;  // Number of pixels per scan line.
};

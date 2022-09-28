#pragma once

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Struct to represent the FrameBuffer used by the Graphics Output
 * Protocol (GOP).
 */
struct FrameBuffer {
    void *BaseAddress;           // Pointer to the beginning address of the FrameBuffer.
    size_t BufferSize;           // Size of the FrameBuffer in bytes.
    uint32_t Width;              // Width of the FrameBuffer in pixels.
    uint32_t Height;             // Height of the FrameBuffer in pixels.
    uint32_t PixelsPerScanLine;  // Number of pixels per scan line.

    /**
     * @brief Clears the contents of the FrameBuffer and returns a pointer to
     * the beginning of the FrameBuffer.
     *
     * @return Pointer to the beginning of the FrameBuffer (BaseAddress).
     */
    void *clear();
};

#pragma once

#include <stdint.h>

/**
 * @brief Struct representing the header of a PSF1 font file.
 */
struct PSF1_Header {
    uint16_t Magic;    // Word containing the PSF1 magic value (0x0436)
    uint8_t Mode;      // Byte value indicating the PSF1 mode of the font.
    uint8_t CharSize;  // Byte value denoting the height of a character in the font.
                       // Width of a character = CharSize / 2
};

/**
 * @brief Struct representing the entirety of a PSF1 font file.
 */
struct PSF1_Font {
    PSF1_Header *Header;  // Pointer to the header of the PSF1 font file.
    void *GlyphBuffer;    // Pointer to the glyph buffer containing the font bitmap.
};

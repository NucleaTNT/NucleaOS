#pragma once

#include <stddef.h>
#include <stdint.h>

#include "FrameBuffer.hpp"
#include "Math.hpp"
#include "font/PSF1.hpp"

/**
 * @brief Class used to print text to the specified targetBuffer.
 */
class TextRenderer {
   private:
    /**
     * @brief The target FrameBuffer to write values to.
     */
    FrameBuffer _targetBuffer;

    /**
     * @brief The PSF1 font to use to determine how to write values to the
     * targetBuffer.
     */
    PSF1_Font _activeFont;

   public:
    /**
     * @brief Construct a new Text Renderer object
     *
     * @param targetBuffer The target FrameBuffer to write values to.
     * @param font The PSF1 font to use to determine how to write values to the
     * targetBuffer.
     * @param colorByte The value of the byte written alongside each character
     * in the targetBuffer denoting some color information about the character.
     */
    TextRenderer(FrameBuffer targetBuffer, PSF1_Font font, uint32_t colorByte);

    /**
     * @brief The current position of the TextRenderer's cursor.
     *
     * A Vector2 used to index into the targetBuffer to dictate where
     * characters should be written to on the screen.
     */
    Vector2<uint32_t> CursorPosition;

    /**
     * @brief The current value of the byte written alongside each character
     * in the targetBuffer denoting some color information about the character.
     */
    uint32_t ActiveColor;

    /**
     * @brief Prints a string to the targetBuffer starting at CursorPosition.
     *
     * @param str Pointer to the string to write to the targetBuffer.
     */
    void _print(const char* str);

    /**
     * @brief Puts a character into the targetBuffer at (xOffset, yOffset)px.
     *
     * @param chr The character to write to the targetBuffer at xOffset,
     * @param colorByte The value of the byte written alongside each character
     * in the targetBuffer denoting some color information about the character.
     * yOffset.
     * @param xOffset The x offset in pixels from (0,0) to write the character
     * to.
     * @param yOffset The y offset in pixels from (0,0) to write the character
     * to.
     */
    void _putChar(const char chr, uint32_t colorByte, uint32_t xOffset, uint32_t yOffset);
};

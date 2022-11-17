#pragma once

#include <stddef.h>
#include <stdint.h>

#include "font/PSF1.hpp"
#include "io/graphics/FrameBuffer.hpp"
#include "utils/Math.hpp"

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
    TextRenderer(FrameBuffer targetBuffer, PSF1_Font font, uint32_t foregroundColor = 0xffffffff, uint32_t backgroundColor = 0x00000000);

    /**
     * @brief The current position of the TextRenderer's cursor.
     *
     * A Vector2 used to index into the targetBuffer to dictate where
     * characters should be written to on the screen.
     */
    Vector2<int64_t> CursorPosition;

    uint32_t ForegroundColor = 0xffffffff;
    uint32_t BackgroundColor = 0x00000000;

    /**
     * @brief Prints characters to the targetBuffer at the CursorPosition.
     *
     * @param chr Character to print.
     */
    void _printChar(const char chr);

    void clearChar();

    /**
     * @brief Prints a string to the targetBuffer starting at CursorPosition.
     *
     * @param str Pointer to the string to write to the targetBuffer.
     */
    void _print(const char* str);

    /**
     * @brief Prints an error message to the targetBuffer at CursorPosition.
     *
     * @param str Pointer to the message to write to the targetBuffer.
     *
     * This method does some additional checks before printing to the screen
     * to ensure the text will be both RED and visible on the screen. Also
     * handles returning the text/background color back to its previous
     * value.
     */
    void _printerr(const char* errStr);

    /**
     * @brief Get the current buffer being written to.
     *
     * @return FrameBuffer object targetBuffer
     */
    FrameBuffer getTargetBuffer();

    /**
     * @brief Calculate the number of characters that can fit on a single line
     * in the targetBuffer.
     *
     * @return The number of characters that can fit on a single line in the
     * targetBuffer.
     */
    size_t getBufferWidthCharacterCount();

    /**
     * @brief Calculate the number of lines that can fit on the screen in the
     * targetBuffer.
     *
     * @return The number of lines that can fit on the screen in the
     * targetBuffer.
     */
    size_t getBufferHeightLineCount();

    /**
     * @brief Clears the screen of any characters and fills it with the
     * provided colorByte instead.
     *
     * @param colorByte The BGRA color byte to fill the screen with.
     */
    void fillColor(uint32_t colorByte);

    /**
     * @brief Puts a character into the targetBuffer at (xOffset, yOffset)px.
     *
     * @param chr The character to write to the targetBuffer at xOffset,
     * @param foregroundColor Color to set pixel if glyph bit is set.
     * @param backgroundColor Color to set pixel if glyph bit is not set.
     * @param xOffset The x offset in pixels from (0,0) to write the character
     * to.
     * @param yOffset The y offset in pixels from (0,0) to write the character
     * to.
     */
    void _putChar(const char chr, uint32_t foregroundColor, uint32_t backgroundColor, uint32_t xOffset, uint32_t yOffset);
};

extern TextRenderer g_TextRenderer;
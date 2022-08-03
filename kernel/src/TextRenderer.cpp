#include "TextRenderer.hpp"

TextRenderer g_TextRenderer((FrameBuffer){}, (PSF1_Font){}, 0);

TextRenderer::TextRenderer(FrameBuffer targetFrameBuffer, PSF1_Font font, uint32_t colorByte = 0xffffffff) {
    _targetBuffer = targetFrameBuffer;
    _activeFont = font;

    ActiveColor = colorByte;
    CursorPosition = {0, 0};
}

void TextRenderer::_putChar(const char chr, uint32_t colorByte, uint32_t xOffset, uint32_t yOffset) {
    uint32_t* pxPtr = (uint32_t*)_targetBuffer.BaseAddress;
    char* fontPtr = (char*)_activeFont.GlyphBuffer + (chr * _activeFont.Header->CharSize);

    /* PSF1 Font CharSize 16 = 8x16 characters */
    for (size_t y = yOffset; y < yOffset + 16; y++) {
        for (size_t x = xOffset; x < xOffset + 8; x++) {
            if ((*fontPtr & (0b10000000 >> (x - xOffset))) > 0) {
                *(uint32_t*)(pxPtr + x + (y * _targetBuffer.PixelsPerScanLine)) = colorByte;
            }
        }
        fontPtr++;
    }
}

void TextRenderer::_print(const char* str) {
    /* Character width and height in pixels. */
    const size_t CHARACTER_WIDTH = _activeFont.Header->CharSize / 2,
                 CHARACTER_HEIGHT = _activeFont.Header->CharSize;

    /* Buffer's width and height in characters / lines. */
    const size_t BUFFER_CHARACTERS_WIDTH = _targetBuffer.PixelsPerScanLine / CHARACTER_WIDTH;
    const size_t BUFFER_CHARACTERS_HEIGHT = _targetBuffer.PixelsPerScanLine / CHARACTER_HEIGHT;

    /* Iterate through string until null terminator reached. */
    for (size_t i = 0; *(str + i) != 0; i++) {
        char chrToPrint = *(str + i);

        /* Handle special characters. */
        switch (chrToPrint) {
            case '\n': {
                CursorPosition.Y += 1;
                continue;
            }

            case '\r': {
                CursorPosition.X = 0;
                continue;
            }
        }

        _putChar(
            chrToPrint,
            ActiveColor,
            CursorPosition.X * CHARACTER_WIDTH,
            CursorPosition.Y * CHARACTER_HEIGHT);

        /*
         * Prevent line overflows. Currently resets to top/beginning once bottom/end of screen/buffer reached.
         * This behaviour is obviously not ideal, but I'll write a better driver/application of this later.
         */
        if ((CursorPosition.X + 1) >= BUFFER_CHARACTERS_WIDTH) {
            CursorPosition = (Vector2<uint32_t>){0, CursorPosition.Y + 1};

            if (CursorPosition.Y > BUFFER_CHARACTERS_HEIGHT) {
                CursorPosition = (Vector2<uint32_t>){0, 0};
            }
        } else {
            CursorPosition.X += 1;
        }
    }
}

FrameBuffer TextRenderer::getTargetBuffer() {
    return _targetBuffer;
}
#include "io/graphics/TextRenderer.hpp"

#include "io/graphics/Colors.hpp"
#include "memory/Memory.hpp"

TextRenderer g_TextRenderer((FrameBuffer){}, (PSF1_Font){}, 0);

TextRenderer::TextRenderer(FrameBuffer targetFrameBuffer, PSF1_Font font, uint32_t foregroundColor, uint32_t backgroundColor) {
    _targetBuffer = targetFrameBuffer;
    _activeFont = font;

    ForegroundColor = foregroundColor;
    BackgroundColor = backgroundColor;
    CursorPosition = {0, 0};
}

void TextRenderer::_putChar(const char chr, uint32_t foregroundColor, uint32_t backgroundColor, uint32_t xOffset, uint32_t yOffset) {
    uint32_t* pxPtr = (uint32_t*)_targetBuffer.BaseAddress;
    char* fontPtr = (char*)_activeFont.GlyphBuffer + (chr * _activeFont.Header->CharSize);

    /* PSF1 Font CharSize 16 = 8x16 characters */
    for (size_t y = yOffset; y < yOffset + 16; y++) {
        for (size_t x = xOffset; x < xOffset + 8; x++) {
            if ((*fontPtr & (0b10000000 >> (x - xOffset))) > 0) {
                *(uint32_t*)(pxPtr + x + (y * _targetBuffer.PixelsPerScanLine)) = foregroundColor;
            } else {
                *(uint32_t*)(pxPtr + x + (y * _targetBuffer.PixelsPerScanLine)) = backgroundColor;
            }
        }
        fontPtr++;
    }
}

void TextRenderer::clearChar() {
    /* Character width and height in pixels. */
    const size_t CHARACTER_WIDTH = _activeFont.Header->CharSize / 2,
                 CHARACTER_HEIGHT = _activeFont.Header->CharSize;

    CursorPosition.X -= 1;
    if (CursorPosition.X < 0) {
        CursorPosition.X = getBufferWidthCharacterCount() - 1;
        CursorPosition.Y -= 1;
        if (CursorPosition.Y < 0) {
            CursorPosition.Y = getBufferHeightLineCount() - 1;
        }
    }

    size_t xOffset = (CursorPosition.X * CHARACTER_WIDTH),
           yOffset = (CursorPosition.Y * CHARACTER_HEIGHT);
    uint32_t* pxPtr = (uint32_t*)_targetBuffer.BaseAddress;

    /* PSF1 Font CharSize 16 = 8x16 characters */
    for (size_t y = yOffset; y < yOffset + CHARACTER_HEIGHT; y++) {
        for (size_t x = xOffset; x < xOffset + CHARACTER_WIDTH; x++) {
            *(uint32_t*)(pxPtr + x + (y * _targetBuffer.PixelsPerScanLine)) = BackgroundColor;
        }
    }
}

void TextRenderer::_printChar(const char chr) {
    /* Character width and height in pixels. */
    const size_t CHARACTER_WIDTH = _activeFont.Header->CharSize / 2,
                 CHARACTER_HEIGHT = _activeFont.Header->CharSize;

    /* Buffer's width and height in characters / lines. */
    const size_t BUFFER_WIDTH_CHARACTER_COUNT = getBufferWidthCharacterCount();
    const size_t BUFFER_HEIGHT_LINE_COUNT = getBufferHeightLineCount();

    /* Handle special characters. */
    switch (chr) {
        case '\n': {
            CursorPosition.Y += 1;

            if (CursorPosition.Y >= BUFFER_HEIGHT_LINE_COUNT) {
                CursorPosition = (Vector2<int64_t>){CursorPosition.X, 0};
            }

            return;
        }

        case '\r': {
            CursorPosition.X = 0;
            return;
        }
    }

    _putChar(
        chr,
        ForegroundColor,
        BackgroundColor,
        CursorPosition.X * CHARACTER_WIDTH,
        CursorPosition.Y * CHARACTER_HEIGHT
    );

    /*
     * Prevent line overflows. Currently resets to top/beginning once bottom/end of screen/buffer reached.
     * This behaviour is obviously not ideal, but I'll write a better driver/application of this later.
     */
    if ((CursorPosition.X + 1) >= BUFFER_WIDTH_CHARACTER_COUNT) {
        CursorPosition = (Vector2<int64_t>){0, CursorPosition.Y + 1};

        if (CursorPosition.Y >= BUFFER_HEIGHT_LINE_COUNT) {
            CursorPosition = (Vector2<int64_t>){0, 0};
        }
    } else {
        CursorPosition.X += 1;
    }
}

void TextRenderer::_print(const char* str) {
    /* Iterate through string until null terminator reached. */
    for (size_t i = 0; *(str + i) != 0; i++) {
        char chrToPrint = *(str + i);
        _printChar(chrToPrint);
    }
}

void TextRenderer::_printerr(const char* errStr) {
    uint32_t tempBGColor = this->BackgroundColor,
             tempFGColor = this->ForegroundColor;

    if (tempBGColor == COLOR_RED) {
        this->BackgroundColor = COLOR_BLACK;
    }
    this->ForegroundColor = COLOR_RED;

    _print(errStr);

    this->BackgroundColor = tempBGColor;
    this->ForegroundColor = tempFGColor;
}

FrameBuffer TextRenderer::getTargetBuffer() {
    return _targetBuffer;
}

void TextRenderer::fillColor(uint32_t colorByte) {
    for (uint32_t y = 0; y < _targetBuffer.Height; y++) {
        for (uint32_t x = 0; x < _targetBuffer.PixelsPerScanLine; x++) {
            *(uint32_t*)(x + (y * _targetBuffer.PixelsPerScanLine) + (uint32_t*)_targetBuffer.BaseAddress) = colorByte;
        }
    }
}

size_t TextRenderer::getBufferWidthCharacterCount() {
    return _targetBuffer.PixelsPerScanLine / (_activeFont.Header->CharSize / 2);
}

size_t TextRenderer::getBufferHeightLineCount() {
    return _targetBuffer.Height / _activeFont.Header->CharSize;
}
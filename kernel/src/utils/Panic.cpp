#include "utils/Panic.hpp"

#include "io/graphics/TextRenderer.hpp"
#include "utils/Str.hpp"

void _panic(const char *panicMessage) {
    g_TextRenderer.ForegroundColor = 0xffffffff;
    g_TextRenderer.BackgroundColor = 0xdeadbeef;
    g_TextRenderer.fillColor(0xdeadbeef);

    const size_t bufferWidth = g_TextRenderer.getBufferWidthCharacterCount();
    const size_t bufferHeight = g_TextRenderer.getBufferHeightLineCount();

    const char *panicMessageHeader = "Whoops! Looks like our kernel was sent into panic.";
    g_TextRenderer.CursorPosition = {(int64_t)((bufferWidth - strlen(panicMessageHeader)) / 2), (int64_t)((bufferHeight / 2) - 1)};
    g_TextRenderer._print(panicMessageHeader);
    g_TextRenderer._print("\n\r");
    g_TextRenderer.CursorPosition = {(int64_t)((bufferWidth - strlen(panicMessage)) / 2), (int64_t)((bufferHeight / 2))};
    g_TextRenderer._print(panicMessage);
}
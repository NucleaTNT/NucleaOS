#pragma once

#include <stdint.h>

namespace QWERTYKeyboard {
    const uint8_t PRESSED_TO_RELEASE_OFFSET = 0x80,
                  BACKSPACE = 0x0e,
                  ENTER = 0x1c,
                  LEFT_SHIFT = 0x2a,
                  RIGHT_SHIFT = 0x36,
                  SPACEBAR = 0x39;

    extern const char ASCIITable[];
    char Translate(uint8_t scanCode, bool isUpperCase);
}  // namespace QWERTYKeyboard

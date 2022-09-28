#include "io/keyboard/PS2_Keyboard.hpp"

#include "io/graphics/TextRenderer.hpp"
#include "io/keyboard/ScancodeTranslation.hpp"

static bool g_IsLeftShiftPressed = false,
            g_IsRightShiftPressed = false;

void handlePS2_Keyboard(uint8_t scanCode) {
    switch (scanCode) {
        case QWERTYKeyboard::LEFT_SHIFT:
            g_IsLeftShiftPressed = true;
            return;

        case QWERTYKeyboard::LEFT_SHIFT + QWERTYKeyboard::PRESSED_TO_RELEASE_OFFSET:
            g_IsLeftShiftPressed = false;
            return;

        case QWERTYKeyboard::RIGHT_SHIFT:
            g_IsRightShiftPressed = true;
            return;

        case QWERTYKeyboard::RIGHT_SHIFT + QWERTYKeyboard::PRESSED_TO_RELEASE_OFFSET:
            g_IsRightShiftPressed = false;
            return;

        case QWERTYKeyboard::ENTER:
            g_TextRenderer._print("\n\r");
            return;

        case QWERTYKeyboard::BACKSPACE:
            g_TextRenderer.clearChar();
            return;

        case QWERTYKeyboard::SPACEBAR:
            g_TextRenderer._printChar(' ');
            return;

        default:
            break;
    }

    char ascii = QWERTYKeyboard::Translate(scanCode, g_IsLeftShiftPressed | g_IsRightShiftPressed);

    if (ascii != 0) {
        g_TextRenderer._printChar(ascii);
    }
}
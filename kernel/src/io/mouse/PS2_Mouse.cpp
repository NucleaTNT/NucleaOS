#include "io/mouse/PS2_Mouse.hpp"

#include "io/IOBus.hpp"
#include "io/graphics/Colors.hpp"
#include "io/graphics/TextRenderer.hpp"
#include "utils/Math.hpp"

#define PS2_LEFT_BTN 0b00000001
#define PS2_MIDDLE_BTN 0b00000010
#define PS2_RIGHT_BTN 0b00000100
#define PS2_X_SIGN 0b00010000
#define PS2_Y_SIGN 0b00100000
#define PS2_X_OVERFLOW 0b01000000
#define PS2_Y_OVERFLOW 0b10000000

bool mouseWait() {
    uint64_t timeout = 10000;
    while (timeout--) {
        if ((inb(0x64) & 0b10) == 0) {
            return true;
        }
    }
    g_TextRenderer._printerr("[mouseWait]: Err -> Mouse timed out.");
    return false;
}

bool mouseWaitInput() {
    uint64_t timeout = 10000;
    while (timeout--) {
        if (inb(0x64) & 0b1) {
            return true;
        }
    }
    g_TextRenderer._printerr("[mouseWaitInput]: Err -> Mouse timed out.");
    return false;
}

uint8_t mouseRead() {
    mouseWaitInput();
    return inb(0x60);
}

void mouseWrite(uint8_t value) {
    mouseWait();
    outb(0x64, 0xd4);
    mouseWait();
    outb(0x60, value);
}

uint8_t MousePacket[4];
size_t MousePacketIndex = 0;
bool IsMousePacketReady = false;
Vector2<uint64_t> MousePosition = {0, 0};
void handlePS2MouseInterrupt(uint8_t data) {
    switch (MousePacketIndex) {
        case 0:
            if (IsMousePacketReady || (data & 0b00001000) == 0) break;
            MousePacket[0] = data;
            MousePacketIndex++;
            break;
        case 1:
            if (IsMousePacketReady) break;
            MousePacket[1] = data;
            MousePacketIndex++;
            break;
        case 2:
            if (IsMousePacketReady) break;
            MousePacket[2] = data;
            IsMousePacketReady = true;
            MousePacketIndex = 0;
            break;
    }
}

void initPS2Mouse() {
    outb(0x64, 0xa8);  // Enable auxiliary device - mouse.

    mouseWait();
    outb(0x64, 0x20);  // Notify keyboard controller we want to send a command
                       // to the mouse.

    mouseWaitInput();
    uint8_t status = inb(0x60);
    status |= 0b10;

    mouseWait();
    outb(0x64, 0x60);

    mouseWait();
    outb(0x60, status); // "COMPAQ"

    mouseWait();
    mouseWrite(0xf6);
    mouseRead();
    mouseWrite(0xf4);
    mouseRead();
}

void processPS2MousePacket() {
    if (!IsMousePacketReady) return;
    IsMousePacketReady = false;

    bool isXNegative = (MousePacket[0] & PS2_X_SIGN),
         isYNegative = (MousePacket[0] & PS2_Y_SIGN),
         isXOverflowed = (MousePacket[0] & PS2_X_OVERFLOW),
         isYOverflowed = (MousePacket[0] & PS2_Y_OVERFLOW);

    MousePosition.X += (isXNegative ? MousePacket[1] - 256 : MousePacket[1]);
    if (isXOverflowed) MousePosition.X += (255 * (isXNegative ? -1 : 1));
    MousePosition.Y -= (isYNegative ? MousePacket[2] - 256 : MousePacket[2]);
    if (isYOverflowed) MousePosition.Y -= (255 * (isYNegative ? -1 : 1));

    MousePosition.X = math::clamp((uint32_t)MousePosition.X, (uint32_t)0, g_TextRenderer.getTargetBuffer().Width - 8);
    MousePosition.Y = math::clamp((uint32_t)MousePosition.Y, (uint32_t)0, g_TextRenderer.getTargetBuffer().Height - 16);

    g_TextRenderer._putChar('.', COLOR_GREEN, COLOR_GREEN, MousePosition.X, MousePosition.Y);
}
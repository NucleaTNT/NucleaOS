#include "VGA.h"

const static size_t NUM_COLS = 80, NUM_ROWS = 25;

VGAOutput::VGAOutput() {
    Buffer = (VGAChar*)0xb8000;
    CursorPos.x = 0;
    CursorPos.y = 0;
    Color = VGAColor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
}

void VGAOutput::ClearRow(uint8_t row) {
    VGAChar empty = {' ', Color};
    for (size_t col = 0; col < NUM_COLS; col++) Buffer[col + NUM_COLS * row] = empty;
}

void VGAOutput::ClearScreen() {
    for (size_t i = 0; i < NUM_ROWS; i++) ClearRow(i);
}

void VGAOutput::ChangeBackgroundColor(uint8_t bgCol) {
    for (size_t row = 0; row < NUM_ROWS; row++) {
        for (size_t col = 0; col < NUM_COLS; col++) Buffer[col + NUM_COLS * row].color = (Color & 0x0f) + (bgCol << 4);
    }
}

void VGAOutput::PrintNewLine() {
    if (CursorPos.y < NUM_ROWS - 1) {
        CursorPos.y++;
        return;
    }

    for (size_t row = 1; row < NUM_ROWS; row++) {
        for (size_t col = 0; col < NUM_COLS; col++) {
            VGAChar character = Buffer[col + NUM_COLS * row];
            Buffer[col + NUM_COLS * (row - 1)] = character;
        }
    }

    ClearRow(NUM_ROWS - 1);
}

void VGAOutput::PrintChar(char chr) {
    switch (chr) {
        case '\n':
            PrintNewLine();
            return;

        case '\r':
            CursorPos.x = 0;
            return;
    
        case '\t':
            Buffer[CursorPos.x + NUM_COLS * CursorPos.y] = {(uint8_t)'|', Color};        
            CursorPos.x += 4;
            if (CursorPos.x > NUM_COLS) PrintString("\n\r");
            return;
    }

    if (CursorPos.x > NUM_COLS) PrintNewLine();
    Buffer[CursorPos.x + NUM_COLS * CursorPos.y] = {(uint8_t)chr, Color};

    CursorPos.x++;
}

void VGAOutput::PrintString(const char* string) {
    for (size_t i = 0; string[i] != 0; i++) PrintChar(string[i]);
}

void VGAOutput::SetClearColor(uint8_t color) {
    Color = VGAColor(Color & 0x0f, color);
}

void VGAOutput::SetColor(uint8_t fgCol, uint8_t bgCol) {
    if (fgCol > VGA_COLOR_WHITE) fgCol = (Color & 0x0f);
    if (fgCol > VGA_COLOR_WHITE) bgCol = (Color >> 4);

    Color = VGAColor(fgCol, bgCol);
}

void VGAOutput::SetPosition(uint8_t col, uint8_t row) {
    CursorPos.x = col;
    CursorPos.y = row;
}

void VGAOutput::SetPosition(Point pos) {
    CursorPos.x = Abs(pos.x);
    CursorPos.y = Abs(pos.y);
}
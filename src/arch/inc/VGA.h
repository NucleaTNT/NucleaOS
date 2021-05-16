#pragma once
#include <stddef.h>
#include <stdint.h>
#include "Math.h"

#define VGAColor(fgCol, bgCol) (fgCol + (bgCol << 4)) 

enum {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE,
    VGA_COLOR_GREEN,
    VGA_COLOR_CYAN,
    VGA_COLOR_RED,
    VGA_COLOR_MAGENTA,
    VGA_COLOR_BROWN,
    VGA_COLOR_LIGHT_GRAY,
    VGA_COLOR_DARK_GRAY,
    VGA_COLOR_LIGHT_BLUE,
    VGA_COLOR_LIGHT_GREEN,
    VGA_COLOR_LIGHT_CYAN,
    VGA_COLOR_LIGHT_RED,
    VGA_COLOR_PINK,
    VGA_COLOR_YELLOW,
    VGA_COLOR_WHITE
};

typedef struct {
    uint8_t character;
    uint8_t color;
} VGAChar;

class VGAOutput {
    private:
        VGAChar* Buffer;
        Point CursorPos;
        uint8_t Color;

        void PrintNewLine();

    public:
        VGAOutput();

        void ChangeBackgroundColor(uint8_t bgCol);
        void ClearScreen();
        void ClearRow(uint8_t row);
        void PrintChar(char chr);
        void PrintString(const char* string);
        void SetClearColor(uint8_t color);
        void SetColor(uint8_t fgCol = 0xff, uint8_t bgCol = 0xff);  // Temp solution for lack of static vars
        void SetPosition(uint8_t col, uint8_t row);
        void SetPosition(Point pos);
};
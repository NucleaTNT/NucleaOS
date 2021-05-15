#pragma once
#include <stddef.h>
#include <stdint.h>

enum {
    PRINT_COLOR_BLACK = 0,
    PRINT_COLOR_BLUE,
    PRINT_COLOR_GREEN,
    PRINT_COLOR_CYAN,
    PRINT_COLOR_RED,
    PRINT_COLOR_MAGENTA,
    PRINT_COLOR_BROWN,
    PRINT_COLOR_LIGHT_GRAY,
    PRINT_COLOR_DARK_GRAY,
    PRINT_COLOR_LIGHT_BLUE,
    PRINT_COLOR_LIGHT_GREEN,
    PRINT_COLOR_LIGHT_CYAN,
    PRINT_COLOR_LIGHT_RED,
    PRINT_COLOR_PINK,
    PRINT_COLOR_YELLOW,
    PRINT_COLOR_WHITE
};

void ClearScreen();
void ChangeScreenColor(uint8_t fgCol, uint8_t bgCol);
void PrintChar(char chr);
void PrintString(const char* string);
void SetClearColor(uint8_t color);
void SetPrintColor(uint8_t fgCol, uint8_t bgCol);
#include "Screen.h"

const static size_t NUM_COLS = 80, NUM_ROWS = 25;

typedef struct {
    uint8_t character;
    uint8_t color;
} VGAChar;

VGAChar* Buffer = (VGAChar*)0xb8000;
size_t CurCol = 0, CurRow = 0;
uint8_t Color = PRINT_COLOR_WHITE | (PRINT_COLOR_BLACK << 4);

void ClearRow(size_t row) {
    VGAChar empty = {' ', Color};
    for (size_t col = 0; col < NUM_COLS; col++) Buffer[col + NUM_COLS * row] = empty;
}

void ClearScreen() {
    for (size_t i = 0; i < NUM_ROWS; i++) ClearRow(i);
}

void ChangeScreenColor(uint8_t fgCol, uint8_t bgCol) {
    for (size_t row = 0; row < NUM_ROWS; row++) {
        for (size_t col = 0; col < NUM_COLS; col++) Buffer[col + NUM_COLS * row].color = fgCol + (bgCol << 4);
    }
}

void PrintNewLine() {
    if (CurRow < NUM_ROWS - 1) {
        CurRow++;
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

void PrintReturn() {
    CurCol = 0;
}

void PrintChar(char chr) {
    switch (chr) {
        case '\n':
            PrintNewLine();
            return;

        case '\r':
            PrintReturn();
            return;
    }

    if (CurCol > NUM_COLS) PrintNewLine();
    Buffer[CurCol + NUM_COLS * CurRow] = {(uint8_t)chr, Color};

    CurCol++;
}

void PrintString(const char* string) {
    for (size_t i = 0; string[i] != 0; i++) PrintChar(string[i]);
}

void SetClearColor(uint8_t color) {
    Color = (Color & 0x0f) + (color << 4);
}

void SetPrintColor(uint8_t fgCol, uint8_t bgCol) {
    Color = fgCol + (bgCol << 4);
}
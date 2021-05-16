#include "VGA.h"

extern "C" void KernelEntry() {
    VGAOutput vgaOutput = VGAOutput();

    vgaOutput.SetColor(VGA_COLOR_WHITE, VGA_COLOR_CYAN);
    vgaOutput.ClearScreen();
    vgaOutput.PrintString("Welcome!\n\r");
}
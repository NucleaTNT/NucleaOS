#include "Math.h"
#include "String.h"
#include "VGA.h"

extern "C" void KernelEntry() {
    VGAOutput vgaOutput = VGAOutput();

    vgaOutput.SetColor(VGA_COLOR_WHITE, VGA_COLOR_CYAN);
    vgaOutput.ClearScreen();

    vgaOutput.PrintString("Welcome!\n\r");
    vgaOutput.PrintString("Hex: ");
    vgaOutput.PrintString(ToHexString((uint32_t)0xC0DEBEEF));
    vgaOutput.PrintString("\n\r");
    vgaOutput.PrintString(ToHexString((uint16_t)0xBABE));
    vgaOutput.PrintString("\n\r");
    vgaOutput.PrintString(ToHexString((uint64_t)0xC0FFEEBABE));
    vgaOutput.PrintString("\n\rUnsigned Int: ");
    vgaOutput.PrintString(ToString((uint64_t)234039));
	vgaOutput.PrintString("\n\rSigned Int: ");
    vgaOutput.PrintString(ToString((int64_t)-389457));
    vgaOutput.PrintString("\n\rDouble: ");

    while (true);
    vgaOutput.PrintString(ToString((double)34.458, 3));

    while(true);
}
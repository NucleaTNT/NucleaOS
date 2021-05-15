#include "../inc/Screen.h"

extern "C" void KernelEntry() {
    SetClearColor(PRINT_COLOR_CYAN);
    ClearScreen();
    
    SetPrintColor(PRINT_COLOR_YELLOW, PRINT_COLOR_CYAN);
    PrintString("[KERNEL] ~ Hiiiii!\n\r");

    SetPrintColor(PRINT_COLOR_RED, PRINT_COLOR_CYAN);
    PrintString("[KERNEL] ~ Bye!");
}
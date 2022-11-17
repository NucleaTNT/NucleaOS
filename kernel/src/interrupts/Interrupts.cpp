#include "interrupts/Interrupts.hpp"

#include "interrupts/PIC.hpp"
#include "io/IOBus.hpp"
#include "io/graphics/TextRenderer.hpp"
#include "io/keyboard/PS2_Keyboard.hpp"
#include "io/mouse/PS2_Mouse.hpp"
#include "memory/Memory.hpp"
#include "utils/Panic.hpp"

__attribute__((interrupt)) void handler_GeneralProtectionFault(InterruptFrame *frame) {
    _panic("General Protection Fault Detected!");

    while (true) {
    }
}

__attribute__((interrupt)) void handler_DoubleFault(InterruptFrame *frame) {
    _panic("Double Fault Detected!");

    while (true) {
    }
}

__attribute__((interrupt)) void handler_PageFault(InterruptFrame *frame) {
    _panic("Page Fault Detected!");

    while (true) {
    }
}

__attribute__((interrupt)) void handler_KeyboardInterrupt(InterruptFrame *frame) {
    /*
     * Read from PS2 Keyboard port, handle it and signal to PIC that the
     * interrupt was handled.
     */
    uint8_t scanCode = inb(0x60);
    handlePS2_Keyboard(scanCode);

    pic_EndMaster();
}

__attribute__((interrupt)) void handler_MouseInterrupt(InterruptFrame *frame) {
    handlePS2MouseInterrupt(inb(0x60));
    pic_EndSlave();
}
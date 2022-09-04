#pragma once

struct InterruptFrame;
__attribute__((interrupt)) void handler_DoubleFault(InterruptFrame *frame);             // Interrupt Offset: 0x8
__attribute__((interrupt)) void handler_GeneralProtectionFault(InterruptFrame *frame);  // Interrupt Offset: 0xd
__attribute__((interrupt)) void handler_PageFault(InterruptFrame *frame);               // Interrupt Offset: 0xe
__attribute__((interrupt)) void handler_KeyboardInterrupt(InterruptFrame *frame);       // Interrupt Offset: 0xe
#pragma once
#include <stdint.h>

#define IDT_TYPES_ATTRS_INTERRUPT_GATE 0b10001110
#define IDT_TYPES_ATTRS_CALL_GATE 0b10001100
#define IDT_TYPES_ATTRS_TRAP_GATE 0b10001111

struct InterruptDescriptor {
    uint16_t Offset_0;   // Offset bits[0..15].
    uint16_t Selector;   // Code segment selector in GDT.
    uint8_t ISTOffset;   // Bits[0..2] hold Interrupt Stack Table offset, bits[3..7] are zeroed.
    uint8_t TypesAttrs;  // Bits[0..3] -> Gate type, [4..6] -> cpu_privilege_level, [7] -> present.
    uint16_t Offset_1;   // Offset bits[16..31].
    uint32_t Offset_2;   // Offset bits[32..63].
    uint32_t _IGNORE;    // Reserved.

    void setOffset(uint64_t offset);
    uint64_t getOffset();
};

struct IDTRegister {
    uint16_t Size;    // One less than size of IDT in bytes
    uint64_t Offset;  // Linear address of Interrupt Descriptor Table (not physAddr).
} __attribute__((packed));

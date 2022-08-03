#include "interrupts/InterruptDescriptorTable.hpp"

uint64_t InterruptDescriptor::getOffset() {
    return ((uint64_t)Offset_0 | ((uint64_t)Offset_1 << 16) | ((uint64_t)Offset_2 << 32));
}

void InterruptDescriptor::setOffset(uint64_t offset) {
    Offset_0 = (uint16_t)(offset & 0x000000000000ffff);
    Offset_1 = (uint16_t)((offset & 0x00000000ffff0000) >> 16);
    Offset_2 = (uint32_t)((offset & 0xffffffff00000000) >> 32);
}
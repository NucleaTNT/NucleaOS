#include "io/IOBus.hpp"

uint8_t inb(uint16_t port) {
    uint8_t value;
    asm volatile("inb %1, %0"
                 : "=a"(value)
                 : "Nd"(port));
    return value;
}

void outb(uint16_t port, uint8_t value) {
    asm volatile("outb %0, %1"
                 :
                 : "a"(value), "Nd"(port));
}

void io_wait() {
    // Waste a single IO cycle by writing to an unused port so that older
    // hardware have time to catch up.
    asm volatile("outb %%al, $0x80"
                 :
                 : "a"(0));
}
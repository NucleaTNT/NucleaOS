#include "interrupts/PIC.hpp"

#include <stdint.h>

#include "io/IOBus.hpp"

void remapPIC() {
    uint8_t picMasterMask, picSlaveMask;

    /* Retrieve both PIC chip masks. */
    picMasterMask = inb(PIC_MASTER_DATA);
    io_wait();
    picSlaveMask = inb(PIC_SLAVE_DATA);
    io_wait();

    /* Initialize PIC chips. */
    outb(PIC_MASTER_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC_SLAVE_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    /* Tell PIC chips their interrupt offsets. */
    outb(PIC_MASTER_DATA, 0x20);
    io_wait();
    outb(PIC_SLAVE_DATA, 0x28);
    io_wait();

    /* Inform PIC chips of eachother's existence. */
    outb(PIC_MASTER_DATA, 4);
    io_wait();
    outb(PIC_SLAVE_DATA, 2);
    io_wait();

    /* Tell PIC chips to operate in 8086 mode. */
    outb(PIC_MASTER_DATA, ICW4_8086);
    io_wait();
    outb(PIC_SLAVE_DATA, ICW4_8086);
    io_wait();

    /* Restore PIC chip masks. */
    outb(PIC_MASTER_DATA, picMasterMask);
    io_wait();
    outb(PIC_SLAVE_DATA, picSlaveMask);
    io_wait();
}

void pic_EndMaster() {
    outb(PIC_MASTER_COMMAND, PIC_EOI);
}

void pic_EndSlave() {
    outb(PIC_SLAVE_COMMAND, PIC_EOI);
    outb(PIC_MASTER_COMMAND, PIC_EOI);
}
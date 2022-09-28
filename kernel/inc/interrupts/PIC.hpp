#pragma once

#define PIC_MASTER_COMMAND 0x20
#define PIC_MASTER_DATA 0x21

#define PIC_SLAVE_COMMAND 0xa0
#define PIC_SLAVE_DATA 0xa1

#define PIC_EOI 0x20

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

/**
 * @brief Remaps the Programmable Interrupt Controller chips so they don't
 * interfere with our IDT and can be used reliably.
 */
void remapPIC();

/**
 * @brief Signals to the Master PIC chip that it's interrupt has been handled.
 */
void pic_EndMaster();

/**
 * @brief Signals to the Slave PIC chip that it's interrupt has been handled.
 */
void pic_EndSlave();
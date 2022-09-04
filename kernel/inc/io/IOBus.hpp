#pragma once

#include <stdint.h>

/**
 * @brief Reads the byte value found at the specified port.
 *
 * @param port The port on the IO bus to read a value from.
 * @return The byte value read from the specified port.
 */
uint8_t inb(uint16_t port);

/**
 * @brief Writes a byte value to the specified port.
 *
 * @param port The port on the IO bus to write to.
 * @param value The byte value to write to the specified port.
 */
void outb(uint16_t port, uint8_t value);

/**
 * @brief Wastes/waits an IO cycle.
 *
 * This is used so that older/slower hardware has a chance to catch up after
 * having a value written to/read from their ports. By sending a 0 to the
 * unused port 0x80 we successfully "waste" an IO cycle, allowing the
 * aforementioned hardware to catch up and update any appropriate values.
 */
void io_wait();
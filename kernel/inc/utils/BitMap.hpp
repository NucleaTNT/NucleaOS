#pragma once
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Class used to store many boolean values more efficiently using single
 * bits instead of entire bytes.
 */
class BitMap {
   public:
    /**
     * @brief Construct a new Bit Map object
     *
     * Buffer and size are passed in and made private so they cannot be altered
     * at a later time.
     *
     * @param buffer Pointer to the region of memory used to store the bitmap's
     * values in.
     * @param size Size of the buffer to use for the BitMap.
     */
    BitMap(uint8_t *buffer, size_t size);

    /**
     * @brief Wrapper of Get(index) to support use of subscript operator.
     *
     * @param index Index of the bit to select. Ranges from [0.._size * 8]
     *
     * @return State of the bit at index. (true->set || false->unset)
     * @return Will also return false if the index was not valid.
     */
    bool operator[](uint64_t index);

    /**
     * @brief Returns the value of the bit found at index.
     *
     * @param index Index of the bit to select. Ranges from [0.._size * 8]
     *
     * @return State of the bit at index. (true->set || false->unset)
     * @return Will also return false if the index was not valid.
     */
    bool Get(uint64_t index);

    /**
     * @brief Sets the bit found at index to value.
     *
     * @param index Index of the bit to select. Ranges from [0.._size * 8]
     *
     * @return True if bit was set successfully else false if the index was not
     * valid.
     */
    bool Set(uint64_t index, bool value);

    /**
     * @brief Get the size of the bitmap's buffer. Multiply this value by 8 to
     * obtain the maximum index.
     *
     * @return The size of the bitmap's buffer.
     */
    size_t getSize();

   private:
    /**
     * @brief Pointer to the region of memory used to store the bitmap's values
     * in.
     */
    uint8_t *_buffer;

    /**
     * @brief Size of the BitMap's buffer in BYTES.
     */
    size_t _size;
};

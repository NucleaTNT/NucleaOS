#include "utils/BitMap.hpp"

#include "memory/Memory.hpp"

BitMap::BitMap(uint8_t *buffer, size_t size) {
    _buffer = buffer;
    _size = size;
    memset(buffer, 0, size);
}

bool BitMap::operator[](uint64_t index) {
    return Get(index);
}

bool BitMap::Get(uint64_t index) {
    if (index > (_size * 8)) {
        return false;
    }

    uint64_t byteIndex = index / 8;
    uint8_t bitIndex = index % 8;
    uint8_t bitMask = 0b10000000 >> bitIndex;

    return ((_buffer[byteIndex] & bitMask) > 0);
}

bool BitMap::Set(uint64_t index, bool value) {
    if (index > (_size * 8)) {
        return false;
    }

    uint64_t byteIndex = index / 8;
    uint8_t bitIndex = index % 8;
    uint8_t bitMask = 0b10000000 >> bitIndex;

    _buffer[byteIndex] &= ~bitMask;
    if (value) {
        _buffer[byteIndex] |= bitMask;
    }

    return true;
}

size_t BitMap::getSize() {
    return _size;
}
#include "io/graphics/FrameBuffer.hpp"

#include "memory/Memory.hpp"

void *FrameBuffer::clear() {
    return memset(BaseAddress, 0, BufferSize);
};
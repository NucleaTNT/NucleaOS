#include "interrupts/Interrupts.hpp"

#include "TextRenderer.hpp"
#include "memory/Memory.hpp"

__attribute__((interrupt)) void handler_PageFault(InterruptFrame *frame) {
    FrameBuffer screenFrameBuff = g_TextRenderer.getTargetBuffer();
    memset(screenFrameBuff.BaseAddress, 0xff, screenFrameBuff.BufferSize);

    while (true) {
    }
}
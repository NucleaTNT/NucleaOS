#pragma once

struct InterruptFrame;
__attribute__((interrupt)) void handler_PageFault(InterruptFrame *frame);
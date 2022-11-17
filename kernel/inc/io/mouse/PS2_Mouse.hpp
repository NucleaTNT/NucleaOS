#pragma once
#include <stdint.h>
#include "utils/Math.hpp"

void handlePS2MouseInterrupt(uint8_t data);
void initPS2Mouse();
void processPS2MousePacket();

extern Vector2<uint64_t> MousePosition;
#pragma once
#include <stddef.h>
#include <stdint.h>

#define Abs(x) (x < 0) ? x * -1 : x

typedef struct {
	size_t x, y;
} Point;
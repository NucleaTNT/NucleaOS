#pragma once
#include <stddef.h>
#include <stdint.h>

//#define Abs(x) (x < 0) ? x * -1 : x

typedef struct {
	size_t x, y;
} Point;

template <typename T>
T Abs(T value) {
	if (value < 0) return value * -1;
	return value;
}
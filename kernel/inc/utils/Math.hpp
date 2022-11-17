#pragma once

#include <stdint.h>

template<typename T>
struct Vector2 {
    T X, Y;
};

namespace math {
    template<typename T>
    T clamp(const T &value, const T &low, const T &high) {
        return value < low ? low : (value > high ? high : value);
    }
}
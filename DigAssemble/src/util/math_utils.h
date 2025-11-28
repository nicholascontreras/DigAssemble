#pragma once

#include <cmath>

inline float loopMod(float x, float y) {
    return fmodf(x + y, y);
}

inline int floorDiv(int x, int y) {
    return (int)std::floor(x / (double)y);
}
#pragma once

#include <cmath>

inline int loopMod(int x, int y) {
    return (x + y) % y;
}

inline int floorDiv(int x, int y) {
    return (int)std::floor(x / (double)y);
}
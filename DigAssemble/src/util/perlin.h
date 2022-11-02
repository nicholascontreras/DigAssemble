#pragma once

#include <math.h>

namespace perlin {

    struct Vec2 {
        float x, y;
    };

    /* Function to linearly interpolate between a0 and a1
     * Weight w should be in the range [0.0, 1.0]
     */
    inline float interpolate(float a0, float a1, float w) {
        /* // You may want clamping by inserting:
         * if (0.0 > w) return a0;
         * if (1.0 < w) return a1;
         */
        return (a1 - a0) * w + a0;
        /* // Use this cubic interpolation [[Smoothstep]] instead, for a smooth appearance:
         * return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
         *
         * // Use [[Smootherstep]] for an even smoother result with a second derivative equal to zero on boundaries:
         * return (a1 - a0) * ((w * (w * 6.0 - 15.0) + 10.0) * w * w * w) + a0;
         */
    }

    /* Create pseudorandom direction vector
     */
    inline Vec2 randomGradient(int ix, int iy, int seed) {
        std::default_random_engine engine(seed);
        std::uniform_int_distribution<unsigned int> generator(1000000000U, 4294967295U);

        // No precomputed gradients mean this works for any number of grid coordinates
        const unsigned int w = 8 * sizeof(unsigned int);
        const unsigned int s = w / 2; // rotation width
        unsigned int a = ix, b = iy;

        //a *= 3284157443; 
        a *= generator(engine);
        b ^= a << s | a >> (w - s);
        //b *= 1911520717; 
        b *= generator(engine);
        a ^= b << s | b >> (w - s);
        //a *= 2048419325;
        a *= generator(engine);
        float random = a * (3.14159265f / ~(~0u >> 1)); // in [0, 2*Pi]
        Vec2 v;
        v.x = cos(random); v.y = sin(random);
        return v;
    }

    // Computes the dot product of the distance and gradient vectors.
    inline float dotGridGradient(int ix, int iy, float x, float y, int seed) {
        // Get gradient from integer coordinates
        Vec2 gradient = randomGradient(ix, iy, seed);

        // Compute the distance vector
        float dx = x - (float)ix;
        float dy = y - (float)iy;

        // Compute the dot-product
        return (dx * gradient.x + dy * gradient.y);
    }

    // Compute Perlin noise at coordinates x, y
    inline float get(int _x, int _y, int scale, int seed) {
        float x = (float)_x / scale;
        float y = (float)_y / scale;

        // Determine grid cell coordinates
        int x0 = (int)floor(x);
        int x1 = x0 + 1;
        int y0 = (int)floor(y);
        int y1 = y0 + 1;

        // Determine interpolation weights
        // Could also use higher order polynomial/s-curve here
        float sx = x - (float)x0;
        float sy = y - (float)y0;

        // Interpolate between grid point gradients
        float n0, n1, ix0, ix1, value;

        n0 = dotGridGradient(x0, y0, x, y, seed);
        n1 = dotGridGradient(x1, y0, x, y, seed);
        ix0 = interpolate(n0, n1, sx);

        n0 = dotGridGradient(x0, y1, x, y, seed);
        n1 = dotGridGradient(x1, y1, x, y, seed);
        ix1 = interpolate(n0, n1, sx);

        value = interpolate(ix0, ix1, sy);
        return value; // Will return in range -1 to 1.
    }
}
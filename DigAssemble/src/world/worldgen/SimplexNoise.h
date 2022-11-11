/**
 * @file    SimplexNoise.h
 * @brief   A Perlin Simplex Noise C++ Implementation (1D, 2D, 3D).
 *
 * Copyright (c) 2014-2018 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#pragma once

#include <cstdint>  // int32_t/uint8_t
#include <cstddef>  // size_t

/**
 * @brief A Perlin Simplex Noise C++ Implementation (1D, 2D, 3D, 4D).
 */
class SimplexNoise {
public:

    // Fractal/Fractional Brownian Motion (fBm) noise summation
    static float fractal(size_t octaves, int x, int scalePow);
    static float fractal(size_t octaves, int x, int y, int scalePow);
    static float fractal(size_t octaves, int x, int y, int z, int scalePow);

private:
    // Parameters of Fractional Brownian Motion (fBm) : sum of N "octaves" of noise
    static uint32_t seed;
    static float frequency;   ///< Frequency ("width") of the first octave of noise (default to 1.0)
    static float amplitude;   ///< Amplitude ("height") of the first octave of noise (default to 1.0)
    static float lacunarity;  ///< Lacunarity specifies the frequency multiplier between successive octaves (default to 2.0).
    static float persistence; ///< Persistence is the loss of amplitude between successive octaves (usually 1/lacunarity)

    // 1D Perlin simplex noise
    static float noise(float x);
    // 2D Perlin simplex noise
    static float noise(float x, float y);
    // 3D Perlin simplex noise
    static float noise(float x, float y, float z);
};

#pragma once

#include <map>

struct Biome {
    enum class BiomeType {
        GRASSLANDS,
        MOUNTAINS,
        NUM_BIOME_TYPES
    };
    
    static Biome at(int x, int z, int seed);

    const BiomeType biomeType;
    const float strength;

    inline Biome(BiomeType biomeType, float strength) : biomeType(biomeType), strength(strength) {};
};
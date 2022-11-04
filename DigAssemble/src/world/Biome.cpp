#include "Biome.h"

#include "worldgen/SimplexNoise.h"

Biome Biome::at(int x, int z, int seed) {
    
    int noiseScale = 64;
    int seedJump = noiseScale * 3;

    BiomeType biomeType = BiomeType::NUM_BIOME_TYPES;
    float strength = 0;
    float nextStrength = 0;
    
    for(int i = 0; i < (int)BiomeType::NUM_BIOME_TYPES; i++) {
        float value = SimplexNoise::normalizedNoise(x, z, noiseScale, seed + (seedJump * i));
        if(value > strength) {
            nextStrength = strength;
            strength = value;
            biomeType = (BiomeType)i;
        }
    }

    return { biomeType, strength - nextStrength };
}

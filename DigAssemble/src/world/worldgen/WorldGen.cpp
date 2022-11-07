#include "WorldGen.h"

#include <algorithm>

#include "SimplexNoise.h"
#include "../Biome.h"

World WorldGen::generateNewWorld(int seed) {
    World world(seed, getTopYCoord(0, 0, seed) + 1);

    for(int x = -1; x < 1; x++) {
        for(int y = -4; y < 8; y++) {
            for(int z = -1; z < 1; z++) {
                generateChunk(world, x, y, z);
                world.getChunk(x, y, z)->sendGeometryToGraphics(world.getChunk(x, y, z)->constructLocalGeometry());
            }
        }
    }

    return world;
}

void WorldGen::generateChunk(World& w, int cx, int cy, int cz) {
    w.initChunk(cx, cy, cz);

    Chunk* c = new Chunk();
    for(int x = 0; x < Chunk::SIZE; x++) {
        for(int z = 0; z < Chunk::SIZE; z++) {
            int maxY = getTopYCoord((cx * Chunk::SIZE) + x, (cz * Chunk::SIZE) + z, w.seed);
            for(int y = 0; y < Chunk::SIZE; y++) {
                if(y + (cy * Chunk::SIZE) <= maxY) {
                    Block* b = generateBlock(x + (cx * Chunk::SIZE), y + (cy * Chunk::SIZE), z + (cz * Chunk::SIZE), w.seed);
                    if(b != nullptr) {
                        c->setBlock(x, y, z, b);
                    }
                }
            }
        }
    }

    Async::lock(w);
    w.loadChunk(cx, cy, cz, c);
    Async::unlock(w);
}

Block* WorldGen::generateBlock(int x, int y, int z, int seed) {
    const int stoneDepth = calcStoneDepth(x, z, seed);
    const int topLevel = getTopYCoord(x, z, seed);

    if(y <= topLevel - stoneDepth) {
        return new Block("stone");
    } else if(y <= topLevel) {
        return new Block("grass");
    } else {
        throw std::invalid_argument("Y value greater than calculated surface level!");
    }
}

int WorldGen::getTopYCoord(int x, int z, int seed) {
    int baseElevation = (int)(SimplexNoise::normalizedNoise(x, z, 128, seed) * 16) + 12;

    float randomFactor = SimplexNoise::normalizedNoise(x, z, 32, seed);
    float mountainFactor = SimplexNoise::normalizedNoise(x, z, 512, seed);

    if(mountainFactor > 0.4f) {
        float mountainHeight = SimplexNoise::normalizedNoise(x, z, 64, seed);
        mountainHeight = std::min(mountainHeight, scale(mountainFactor, 0.4f, 1.0f));
        return baseElevation + (int)((4 * randomFactor) + (100 * mountainHeight));
    } else {
        return baseElevation + (int)(4 * randomFactor);
    }
}

int WorldGen::calcStoneDepth(int x, int z, int seed) {
    return (int) (SimplexNoise::normalizedNoise(x, z, 1, seed) * 5) + 2;
}

float WorldGen::scale(float value, float low, float high) {
    if(value <= low) {
        return 0.0f;
    } else if(value >= high) {
        return 1.0f;
    } else {
        float range = high - low;
        return (value - low) / range;
    }
}


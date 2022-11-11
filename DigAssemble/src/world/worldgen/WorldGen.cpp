#include "WorldGen.h"

#include <algorithm>

#include "../Biome.h"

int WorldGen::seed = 0;

void WorldGen::setSeed(int newSeed) {
    seed = newSeed;
}

World WorldGen::generateNewWorld() {
    World world(seed, getTopYCoord(0, 0) + 1);
    generateChunk(world, -1, 1, -1);
    int x = world.getChunk(-1, 1, -1)->constructLocalGeometry();
    world.getChunk(-1, 1, -1)->sendGeometryToGraphics(x);
    return world;
}

void WorldGen::generateChunk(World& w, int cx, int cy, int cz) {
    setSeed(w.seed);

    Chunk* c = new Chunk();
    for(int x = 0; x < Chunk::SIZE; x++) {
        for(int z = 0; z < Chunk::SIZE; z++) {
            int maxY = getTopYCoord((cx * Chunk::SIZE) + x, (cz * Chunk::SIZE) + z);
            for(int y = 0; y < Chunk::SIZE; y++) {
                if(y + (cy * Chunk::SIZE) <= maxY) {
                    Block* b = generateBlock(x + (cx * Chunk::SIZE), y + (cy * Chunk::SIZE), z + (cz * Chunk::SIZE));
                    if(b != nullptr) {
                        c->setBlock(x, y, z, b);
                    }
                }
            }
        }
    }

    Async::lock(w);
    w.initChunk(cx, cy, cz, c);
    Async::unlock(w);
}

Block* WorldGen::generateBlock(int x, int y, int z) {
    const int stoneDepth = calcStoneDepth(x, z);
    const int topLevel = getTopYCoord(x, z);

    if(y <= topLevel - stoneDepth) {
        return new Block("stone");
    } else if(y <= topLevel) {
        if(Biome::at(x, z).biomeType == Biome::BiomeType::DESERT) {
            return new Block("sand");
        } else {
            return new Block("grass");
        }
    } else {
        throw std::invalid_argument("Y value greater than calculated surface level!");
    }
}

int WorldGen::getTopYCoord(int x, int z) {
    int baseElevation = (int)(SimplexNoise::fractal(2, x, z, 10) * 16) + 12;
    int localElevation = (int)(SimplexNoise::fractal(3, x, z, 6) * 8) - 4;
    return baseElevation + localElevation;
}

int WorldGen::calcStoneDepth(int x, int z) {
    return (int) (SimplexNoise::fractal(1, x, z, 1) * 5) + 2;
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


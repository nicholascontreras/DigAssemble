#include "WorldGen.h"

#include "SimplexNoise.h"
#include "../Biome.h"
#include "../../util/debug.h"
#include "../../util/async/Async.h"
#include "../../util/async/AsyncWorker.h"

int WorldGen::GENERATION_DISTANCE = -1;

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

void WorldGen::start(World& w, const Player& player) {
    Async::startThread([&] {
        Async::lock("worldGen");
        if(!expandWorld(w, player)) {
            Async::unlock("worldGen");
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    });
}

bool WorldGen::expandWorld(World& w, const Player& player) {
    int stepCount = 0;
    int stepDir = 0;
    int stepSize = 1;

    const int cx = Chunk::at(player.getX());
    const int cy = Chunk::at(player.getY());
    const int cz = Chunk::at(player.getZ());

    int curX = cx;
    int curZ = cz;

    while(true) {
        int taxicabDistance = abs(curX - cx) + abs(curZ - cz);
        if(taxicabDistance > GENERATION_DISTANCE) {
            break;
        }

        for(int curY = cy - GENERATION_DISTANCE / 2; curY < cy + GENERATION_DISTANCE / 2; curY++) {
            if(!w.chunkExists(curX, curY, curZ)) {
                AsyncWorker::queue<unsigned int>([&w, curX, curY, curZ, cx, cy, cz]() {
                    Debug("Generating chunk in local memory, x: ", curX, " y: ", curY, " z: ", curZ);
                    generateChunk(w, curX, curY, curZ);
                    return w.getChunk(curX, curY, curZ)->constructLocalGeometry();
                }, [&w, curX, curY, curZ, cx, cy, cz](unsigned int geometryConstructionBufferSizeUsed) {
                    Debug("Sending local chunk to graphics, x: ", curX, " y: ", curY, " z: ", curZ);
                    w.getChunk(curX, curY, curZ)->sendGeometryToGraphics(geometryConstructionBufferSizeUsed);
                        
                    Async::unlock("worldGen");
                });
                return true;
            }
        }

        switch(stepDir) {
        case 0:
            curZ--;
            break;
        case 1:
            curX++;
            break;
        case 2:
            curZ++;
            break;
        case 3:
            curX--;
            break;
        default:
            throw std::runtime_error("Invalid step direction!");
        }

        stepCount++;
        if(stepCount == stepSize) {
            stepDir++;
            stepDir %= 4;
            stepCount = 0;

            if(stepDir % 2 == 0) {
                stepSize++;
            }
        }
    }
    return false;
}

void WorldGen::generateChunk(World& w, int cx, int cy, int cz) {
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
    w.setChunk(cx, cy, cz, c);
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
        mountainHeight = min(mountainHeight, scale(mountainFactor, 0.4f, 1.0f));
        return baseElevation + (int)((4 * randomFactor) + (100 * mountainHeight));
    } else {
        // Very flat plains
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


#include "WorldGen.h"

#include "../util/perlin.h"
#include "../util/debug.h"
#include "../util/AsyncWorker.h"

int WorldGen::GENERATION_DISTANCE = -1;
bool WorldGen::stop = false;
std::thread WorldGen::worldGenThread;
ResourceLock WorldGen::lock;

World WorldGen::generateNewWorld(int seed) {
    World world(seed);

    for(int x = -1; x < 1; x++) {
        for(int y = -1; y < 1; y++) {
            for(int z = -1; z < 1; z++) {
                generateChunk(world, x, y, z);
                world.getChunk(x, y, z)->sendGeometryToGraphics(world.getChunk(x, y, z)->constructLocalGeometry());
            }
        }
    }

    return world;
}

void WorldGen::start(World& w, const Player& player) {
    worldGenThread = std::thread([&]() {
        while(!stop) {
            lock.lock();
            if(!expandWorld(w, player)) {
                lock.unlock();
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
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

        for(int curY = cy - GENERATION_DISTANCE; curY < cy + GENERATION_DISTANCE; curY++) {
            if(!w.chunkExists(curX, curY, curZ)) {
                AsyncWorker::queue<unsigned int>([&w, curX, curY, curZ, cx, cy, cz]() {
                    Debug("Generating chunk in local memory, x: ", curX, " y: ", curY, " z: ", curZ);
                    generateChunk(w, curX, curY, curZ);
                    return w.getChunk(curX, curY, curZ)->constructLocalGeometry();
                }, [&w, curX, curY, curZ, cx, cy, cz](unsigned int geometryConstructionBufferSizeUsed) {
                    Debug("Sending local chunk to graphics, x: ", curX, " y: ", curY, " z: ", curZ);
                    w.getChunk(curX, curY, curZ)->sendGeometryToGraphics(geometryConstructionBufferSizeUsed);
                        
                    lock.unlock();
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
            int maxY = calcY((cx * Chunk::SIZE) + x, (cz * Chunk::SIZE) + z, w.seed);
            for(int y = 0; y < Chunk::SIZE; y++) {
                if(y + (cy * Chunk::SIZE) <= maxY) {
                    c->setBlock(x, y, z, new Block("grass"));
                }
            }
        }
    }
    w.setChunk(cx, cy, cz, c);
}

int WorldGen::calcY(int x, int z, int seed) {
    float base = perlin::get(x, z, 16, seed);
    return (int)(base * 8) + 9;
}

void WorldGen::shutdown() {
    stop = true;
}

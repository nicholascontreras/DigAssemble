#include "WorldGen.h"

#include "../util/perlin.h"
#include "../util/debug.h"

World WorldGen::generateNewWorld(int seed) {
    World world(seed);

    for(int x = -1; x < 1; x++) {
        for(int y = -1; y < 1; y++) {
            for(int z = -1; z < 1; z++) {
                generateChunk(world, x, y, z, false);
            }
        }
    }

    return world;
}

void WorldGen::expandWorldAroundPlayer(World& w, Player& player, int renderDistance) {
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
        if(taxicabDistance > renderDistance) {
            break;
        }

        for(int curY = cy - renderDistance; curY < cy + renderDistance; curY++) {
            if(!w.chunkExists(curX, curY, curZ)) {
                generateChunk(w, curX, curY, curZ, true);
                return;
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
}

void WorldGen::generateChunk(World& w, int cx, int cy, int cz, bool async) {
    Chunk* c = w.getChunk(cx, cy, cz);
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

    Debug("Generated chunk in local memory, x: ", cx, " y: ", cy, " z: ", cz);

    c->buildGeometry(async);
}

int WorldGen::calcY(int x, int z, int seed) {
    float base = perlin::get(x, z, 16, seed);
    return (int)(base * 8) + 9;
}
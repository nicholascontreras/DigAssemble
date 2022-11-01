#pragma once

#include <string>
#include <random>

#include "World.h"
#include "../util/perlin.h"

namespace worldgen {
    inline int calcY(int x, int z, int seed) {
        float base = perlin(x, z, 16, seed);
        return (int)(base * 8) + 9;
    }

    inline void generateChunk(World& w, int cx, int cy, int cz) {
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
        c->buildGeometry();
    }

    inline World generateWorld(int seed) {
        World world(seed);

        for(int x = -1;  x < 1; x++) {
            for(int y = -1; y < 1; y++) {
                for(int z = -1; z < 1; z++) {
                    generateChunk(world, x, y, z);
                }
            }
        }

        return world;
    }

    inline void expandWorldIfNeeded(World& w, int cx, int cy, int cz, int renderDistance) {
        int stepCount = 0;
        int stepDir = 0;
        int stepSize = 1;

        int curX = cx;
        int curZ = cz;

        while(true) {
            int taxicabDistance = abs(curX - cx) + abs(curZ - cz);
            if(taxicabDistance > renderDistance) {
                break;
            }

            for(int curY = cy - renderDistance; curY < cy + renderDistance; curY++) {
                if(!w.chunkExists(curX, curY, curZ)) {
                    Debug(curX, " ,", curY, " ,", curZ);
                    generateChunk(w, curX, curY, curZ);
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
}
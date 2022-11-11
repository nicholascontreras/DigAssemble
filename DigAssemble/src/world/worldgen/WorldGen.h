#pragma once

#include "../World.h"
#include "../Player.h"

#include "SimplexNoise.h"

class WorldGen {
public:
    static void setSeed(int newSeed);

    static World generateNewWorld();
    static void generateChunk(World& w, int cx, int cy, int cz);
private:
    static int seed;

    static Block* generateBlock(int x, int y, int z);

    static int getTopYCoord(int x, int z);
    static int calcStoneDepth(int x, int z);

    static float scale(float value, float low, float high);
};
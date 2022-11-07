#pragma once

#include "../World.h"
#include "../Player.h"

class WorldGen {
public:
    static World generateNewWorld(int seed);
    static void generateChunk(World& w, int cx, int cy, int cz);
private:
    static Block* generateBlock(int x, int y, int z, int seed);

    static int getTopYCoord(int x, int z, int seed);
    static int calcStoneDepth(int x, int z, int seed);

    static float scale(float value, float low, float high);
};
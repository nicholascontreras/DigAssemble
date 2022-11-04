#pragma once

#include <string>
#include <random>
#include <thread>

#include "../World.h"
#include "../Player.h"

class WorldGen {
public:
    static int GENERATION_DISTANCE;

    static World generateNewWorld(int seed);
    static void start(World& w, const Player& player);
private:
    static bool expandWorld(World& w, const Player& player);
    static void generateChunk(World& w, int cx, int cy, int cz);
    static Block* generateBlock(int x, int y, int z, int seed);

    static int getTopYCoord(int x, int z, int seed);
    static int calcStoneDepth(int x, int z, int seed);

    static float scale(float value, float low, float high);
};
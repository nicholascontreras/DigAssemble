#pragma once

#include <string>
#include <random>
#include <thread>

#include "World.h"
#include "Player.h"
#include "../util/ResourceLock.h"

class WorldGen {
public:
    static int GENERATION_DISTANCE;

    static World generateNewWorld(int seed);
    static void start(World& w, const Player& player);
    static void shutdown();
private:

    static bool stop;
    static std::thread worldGenThread;
    static ResourceLock lock;

    static bool expandWorld(World& w, const Player& player);
    static void generateChunk(World& w, int cx, int cy, int cz);
    static int calcY(int x, int z, int seed);
};
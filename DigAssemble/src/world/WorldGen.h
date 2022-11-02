#pragma once

#include <string>
#include <random>

#include "World.h"
#include "Player.h"

class WorldGen {
public:
    static World generateNewWorld(int seed);
    static void expandWorldAroundPlayer(World& w, Player& player, int renderDistance);
private:
    static void generateChunk(World& w, int cx, int cy, int cz, bool async);
    static int calcY(int x, int z, int seed);
};
#pragma once

#include <string>

#include "World.h"
#include "Player.h"

class WorldStreaming {
public:
    static int RENDER_DISTANCE;
    static std::string FILE_NAME;

    static void start(World& world, const Player& player);
private:
    static bool needLoadChunk(const World& world, const Player& player, int& cx, int& cy, int& cz);
    static void createOrStreamChunk(World& world, int cx, int cy, int cz);

    static void unloadChunkIfPossible(World& world, const Player& player);
};
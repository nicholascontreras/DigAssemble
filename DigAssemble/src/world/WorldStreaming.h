#pragma once

#include <string>

#include "World.h"
#include "Player.h"

class WorldStreaming {
public:
    static int CHUNK_LOAD_DISTANCE;
    static std::string SAVE_FOLDER_PATH;

    static void start(World& world, const Player& player);
private:
    static bool needLoadChunk(const World& world, const Player& player, int& cx, int& cy, int& cz);
    static void createOrStreamChunk(World& world, int cx, int cy, int cz);

    static void unloadChunkIfPossible(World& world, const Player& player);
};
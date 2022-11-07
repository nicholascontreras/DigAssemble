#pragma once

#include <string>

#include "World.h"

class WorldFile {
public:
    static void saveWorld(const World& world, const std::string& fileName);
    static void saveChunk(const Chunk* chunk, const std::string& fileName);

    static World loadWorld(const std::string& fileName);
    static Chunk* loadChunk(const std::string& fileName, int cx, int cy, int cz);
private:
};

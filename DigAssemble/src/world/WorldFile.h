#pragma once

#include <string>

#include "World.h"

class WorldFile {
public:
    static void saveWorld(const World& world, const std::string& saveFolderPath);
    static void saveChunk(const Chunk* chunk, const std::string& saveFolderPath, int cx, int cy, int cz);

    static World loadWorld(const std::string& saveFolderPath);
    static Chunk* loadChunk(const std::string& saveFolderPath, int cx, int cy, int cz);
private:
};

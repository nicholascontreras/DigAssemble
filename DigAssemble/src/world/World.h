#pragma once

#include <map>

#include "blocks/Block.h"
#include "Chunk.h"

class World {
public:
    World();
    ~World();

    World(const World& other) = delete;
    World(World&& other) noexcept;
    World& operator=(const World& other) = delete;
    World& operator=(World&& other) = delete;

    friend void swap(World& first, World& second);

    bool chunkExists(int x, int y, int z);
    Chunk& getChunk(int x, int y, int z);

    void draw();
private:
    static const unsigned int STARTING_SIZE = 2;

    std::map<int, std::map<int, std::map<int, Chunk>>> chunks;
};


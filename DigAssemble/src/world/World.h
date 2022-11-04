#pragma once

#include <map>

#include "blocks/Block.h"
#include "Chunk.h"

class World {
public:
    const int seed;
    const int spawnY;

    World(int seed, int spawnY);
    ~World();

    World(const World& other) = delete;
    World(World&& other) noexcept;
    World& operator=(const World& other) = delete;
    World& operator=(World&& other) = delete;

    friend void swap(World& first, World& second);

    bool chunkExists(int x, int y, int z);
    Chunk* getChunk(int x, int y, int z);
    void setChunk(int x, int y, int z, Chunk* c);

    bool blockExists(int x, int y, int z);
    Block* getBlock(int x, int y, int z);
    void setBlock(int x, int y, int z, Block* b);

    void draw();
private:
    std::map<int, std::map<int, std::map<int, Chunk*>>> chunks;
};


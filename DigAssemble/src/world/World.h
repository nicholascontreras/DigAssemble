#pragma once

#include <map>

#include "blocks/Block.h"
#include "Chunk.h"
#include "Coord.h"
#include "Player.h"

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

    bool chunkExists(int x, int y, int z) const;
    bool chunkLoaded(int x, int y, int z) const;
    Chunk* getChunk(int x, int y, int z) const;
    void initChunk(int x, int y, int z, Chunk* c);
    void loadChunk(int x, int y, int z, Chunk* c);
    Chunk* unloadChunk(int x, int y, int z);

    std::unordered_map<Coord, Chunk*> loadedChunks() const;
    std::unordered_map<Coord, Chunk*> allChunks() const;

    void draw();
private:
    std::map<int, std::map<int, std::map<int, Chunk*>>> chunks;
};


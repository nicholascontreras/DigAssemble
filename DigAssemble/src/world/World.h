#pragma once

#include <unordered_map>

#include "blocks/Block.h"

class World {
public:
    World();
    ~World();

    World(const World& other) = delete;
    World(World&& other) = delete;
    World& operator=(const World& other) = delete;
    World& operator=(World&& other) = delete;

    bool isBlock(int x, int y, int z);
    void setBlock(Block* b);
    Block* getBlock(int x, int y, int z);

    void draw();
private:
    std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, Block*>>> blocks;
};


#pragma once

#include <map>

#include "blocks/Block.h"

class World {
public:
    World();
    ~World();

    World(const World& other) = delete;
    World(World&& other) noexcept;
    World& operator=(const World& other) = delete;
    World& operator=(World&& other) = delete;

    friend void swap(World& first, World& second);

    bool isBlock(int x, int y, int z);
    void setBlock(Block* b);
    Block* getBlock(int x, int y, int z);

    void draw();
private:
    std::map<int, std::map<int, std::map<int, Block*>>> blocks;
};


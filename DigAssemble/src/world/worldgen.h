#pragma once
#include "World.h"

namespace worldgen {
    inline World generateWorld() {
        World world;
        world.setBlock(new Block(0, 0, -5));
        world.setBlock(new Block(-5, 0, -5));
        world.setBlock(new Block(-5, 0, 0));
        return world;
    };
}
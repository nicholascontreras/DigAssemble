#pragma once
#include "World.h"

namespace worldgen {
    inline World generateWorld() {
        World world;
        //world.setBlock(new Block(0, -2, -2));
        //world.setBlock(new Block(0, -3, -2));
        //world.setBlock(new Block(0, -4, -2));
        for(int x = -10; x < 10; x++) {
            for(int y = -10; y < 0; y++) {
                for(int z = -10; z < 10; z++) {
                    world.setBlock(new Block(x, y, z));
                }
            }
        }
        return world;
    };
}
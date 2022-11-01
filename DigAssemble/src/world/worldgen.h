#pragma once
#include "World.h"

namespace worldgen {
    inline World generateWorld() {
        World world;

        for(int x = -8;  x < 8; x++) {
            for(int y = -8; y < 8; y++) {
                for(int z = -8; z < 8; z++) {
                    world.setBlock(x, y, z, new Block("container"));
                }
            }
        }

        world.buildAllGeometry();
        return world;
    };
}
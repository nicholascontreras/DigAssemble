#pragma once

#include <glm/glm.hpp>

class Block {
public:
    const int x, y, z;

    static void init();

    Block(int x, int y, int z);

    void draw() const;
private:
    static unsigned int VAO, VBO;
};


#pragma once

#include <glm/glm.hpp>

class Block {
public:
    const static float vertices[];
    const static int numVertices;

    const int x, y, z;

    Block(const int& x, const int& y, const int& z);

    glm::mat4 getModelMat() const;
private:
};


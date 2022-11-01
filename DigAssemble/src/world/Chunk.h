#pragma once

#include <glm/glm.hpp>

#include "blocks/Block.h"

class Chunk {
public:
    static const int SIZE = 16;

    Chunk();
    ~Chunk();
    Chunk(const Chunk& other) = delete;
    Chunk(Chunk&& other) noexcept;
    Chunk& operator=(const Chunk& other) = delete;
    Chunk& operator=(Chunk&& other) = delete;

    friend void swap(Chunk& first, Chunk& second);

    bool isInBounds(int x, int y, int z);
    bool blockExists(int x, int y, int z);
    Block* getBlock(int x, int y, int z);
    void setBlock(int x, int y, int z, Block* b);

    void buildGeometry();
    void draw();
private:
    static const unsigned int geometryConstructionBufferSize = Block::NUM_VERTICES * Block::ELEMENTS_PER_VERTEX * SIZE * SIZE * SIZE;
    static float geometryConstructionBuffer[geometryConstructionBufferSize];

    Block* blocks[SIZE][SIZE][SIZE];

    unsigned int vao, vbo;
    unsigned int vertexCount;
};


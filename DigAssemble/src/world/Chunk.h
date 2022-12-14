#pragma once

#include <glm/glm.hpp>

#include "blocks/Block.h"
#include "../util/async/Async.h"

class Chunk {
public:
    static const int SIZE = 24;

    static int at(float a);

    struct ChunkPos {
        int x, y, z;
    };

    Chunk();
    ~Chunk();
    Chunk(const Chunk& other) = delete;
    Chunk(Chunk&& other) noexcept;
    Chunk& operator=(const Chunk& other) = delete;
    Chunk& operator=(Chunk&& other) = delete;

    friend void swap(Chunk& first, Chunk& second);

    bool isInBounds(int x, int y, int z) const;
    bool blockExists(int x, int y, int z) const;
    Block* getBlock(int x, int y, int z) const;
    void setBlock(int x, int y, int z, Block* b);

    unsigned int constructLocalGeometry();
    void sendGeometryToGraphics(unsigned int geometryConstructionBufferSizeUsed);

    void draw();
private:
    static const unsigned int geometryConstructionBufferSize = (Block::NUM_VERTICES * Block::ELEMENTS_PER_VERTEX * SIZE * SIZE * SIZE) / 2;
    static float geometryConstructionBuffer[geometryConstructionBufferSize];

    Block* blocks[SIZE][SIZE][SIZE];

    unsigned int vao, vbo;
    unsigned int vertexCount;
};


#pragma once

#include <mutex>

#include <glm/glm.hpp>

#include "blocks/Block.h"

class Chunk {
public:
    static const int SIZE = 16;

    static int at(float a);

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

    unsigned int constructLocalGeometry();
    void sendGeometryToGraphics(unsigned int geometryConstructionBufferSizeUsed);

    void draw();
private:
    static const unsigned int geometryConstructionBufferSize = (Block::NUM_VERTICES * Block::ELEMENTS_PER_VERTEX * SIZE * SIZE * SIZE) / 2;
    static float geometryConstructionBuffer[geometryConstructionBufferSize];
    
    static std::mutex geometryConstructionBufferActiveMutex;
    static std::condition_variable geometryConstructionBufferActiveCondition;
    static bool geometryConstructionBufferActive;

    Block* blocks[SIZE][SIZE][SIZE];

    unsigned int vao, vbo;
    unsigned int vertexCount;
};


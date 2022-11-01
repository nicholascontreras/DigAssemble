#include "Chunk.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <stdexcept>

#include "../managers/ShaderProgramManager.h"
#include "../util/GLThread.h"

float Chunk::geometryConstructionBuffer[Chunk::geometryConstructionBufferSize] = {0};

int Chunk::at(float a) {
    return (int) (a / Chunk::SIZE) - (a < 0 ? 1 : 0);
}

Chunk::Chunk() : blocks{ {{nullptr}} }, vao(0), vbo(0), vertexCount(0) {
}

Chunk::~Chunk() {
    for(int x = 0; x < SIZE; x++) {
        for(int y = 0; y < SIZE; y++) {
            for(int z = 0; z < SIZE; z++) {
                delete blocks[x][y][z];
            }
        }
    }

    if(vao) {
        glBindVertexArray(vao);
        if(vbo) {
            glDeleteBuffers(1, &vbo);
        }
        glDeleteVertexArrays(1, &vao);
    }
}

Chunk::Chunk(Chunk&& other) noexcept : Chunk() {
    swap(*this, other);
}

bool Chunk::isInBounds(int x, int y, int z) {
    return x >= 0 && x < SIZE && y >= 0 && y < SIZE && z >= 0 && z < SIZE;
}

bool Chunk::blockExists(int x, int y, int z) {
    if(!isInBounds(x, y, z)) {
        throw std::invalid_argument("Out of bounds!");
    }
    return blocks[x][y][z] != nullptr;
}

Block* Chunk::getBlock(int x, int y, int z) {
    if(!blockExists(x, y, z)) {
        throw std::invalid_argument("No block!");
    }
    return blocks[x][y][z];
}

void Chunk::setBlock(int x, int y, int z, Block* b) {
    if(!isInBounds(x, y, z)) {
        throw std::invalid_argument("Out of bounds!");
    }
    blocks[x][y][z] = b;
}

void Chunk::buildGeometry() {
    GLThread::call([&]() {
        if(!vao) {
            glGenVertexArrays(1, &vao);
        }
        glBindVertexArray(vao);

        if(vbo) {
            glDeleteBuffers(1, &vbo);
            vbo = 0;
        }
    });

    unsigned int geometryConstructionBufferSizeUsed = 0;
    vertexCount = 0;

    for(int x = 0; x < SIZE; x++) {
        glm::mat4 matX = glm::translate(glm::mat4(1), glm::vec3(x, 0, 0));
        for(int y = 0; y < SIZE; y++) {
            glm::mat4 matXY = glm::translate(matX, glm::vec3(0, y, 0));
            for(int z = 0; z < SIZE; z++) {
                if(blockExists(x, y, z)) {
                    glm::mat4 matXYZ = glm::translate(matXY, glm::vec3(0, 0, z));

                    if(!isInBounds(x + 1, y, z) || !blockExists(x + 1, y, z)) {
                        getBlock(x, y, z)->appendPosXFace(geometryConstructionBuffer, geometryConstructionBufferSizeUsed, matXYZ);
                        vertexCount += Block::VERTICES_PER_FACE;
                    }
                    if(!isInBounds(x - 1, y, z) || !blockExists(x - 1, y, z)) {
                        getBlock(x, y, z)->appendNegXFace(geometryConstructionBuffer, geometryConstructionBufferSizeUsed, matXYZ);
                        vertexCount += Block::VERTICES_PER_FACE;;
                    }

                    if(!isInBounds(x, y + 1, z) || !blockExists(x, y + 1, z)) {
                        getBlock(x, y, z)->appendPosYFace(geometryConstructionBuffer, geometryConstructionBufferSizeUsed, matXYZ);
                        vertexCount += Block::VERTICES_PER_FACE;;
                    }
                    if(!isInBounds(x, y - 1, z) || !blockExists(x, y - 1, z)) {
                        getBlock(x, y, z)->appendNegYFace(geometryConstructionBuffer, geometryConstructionBufferSizeUsed, matXYZ);
                        vertexCount += Block::VERTICES_PER_FACE;;
                    }

                    if(!isInBounds(x, y, z + 1) || !blockExists(x, y, z + 1)) {
                        getBlock(x, y, z)->appendPosZFace(geometryConstructionBuffer, geometryConstructionBufferSizeUsed, matXYZ);
                        vertexCount += Block::VERTICES_PER_FACE;;
                    }
                    if(!isInBounds(x, y, z - 1) || !blockExists(x, y, z - 1)) {
                        getBlock(x, y, z)->appendNegZFace(geometryConstructionBuffer, geometryConstructionBufferSizeUsed, matXYZ);
                        vertexCount += Block::VERTICES_PER_FACE;;
                    }
                }
            }
        }
    }

    GLThread::call([&]() {
        if(geometryConstructionBufferSizeUsed > 0) {
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, false, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, false, 5 * sizeof(float), (void*)(3 * sizeof(float)));

            glBufferData(GL_ARRAY_BUFFER, geometryConstructionBufferSizeUsed * sizeof(float), geometryConstructionBuffer, GL_STATIC_DRAW);
        } else {
            glDeleteVertexArrays(1, &vao);
            vao = 0;
        }
    });
}

void Chunk::draw() {
    if(vao) {
        ShaderProgramManager::setActiveProgram("triangle");
        GLThread::call([&]() {
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, vertexCount);
        });
    }
}

void swap(Chunk& first, Chunk& second) {
    using std::swap;

    swap(first.blocks, second.blocks);
    swap(first.vao, second.vao);
    swap(first.vertexCount, second.vertexCount);
}

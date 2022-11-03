#include "World.h"

#include <stdexcept>

#include <glm/gtc/matrix_transform.hpp>

#include "../managers/ShaderProgramManager.h"
#include "../managers/TextureMapManager.h"
#include "../util/math_utils.h"

World::World(int seed) : seed(seed) {
}

World::~World() {
    for(const auto& x : chunks) {
        for(const auto& y : chunks.at(x.first)) {
            for(const auto& z : chunks.at(x.first).at(y.first)) {
                delete z.second;
            }
        }
    }
}

World::World(World&& other) noexcept : World(this->seed) {
    swap(*this, other);
}

bool World::chunkExists(int x, int y, int z) {
    return chunks.count(x) && chunks.at(x).count(y) && chunks.at(x).at(y).count(z);
}

Chunk* World::getChunk(int x, int y, int z) {
    if(!chunkExists(x, y, z)) {
        throw std::invalid_argument("No chunk!");
    }
    return chunks.at(x).at(y).at(z);
}

void World::setChunk(int x, int y, int z, Chunk* c) {
    if(chunkExists(x, y, z)) {
        throw std::invalid_argument("Chunk already exists!");
    }

    if(!chunks.count(x)) {
        chunks.emplace(x, std::map<int, std::map<int, Chunk*>>());
    }
    if(!chunks.at(x).count(y)) {
        chunks.at(x).emplace(y, std::map<int, Chunk*>());
    }
    chunks.at(x).at(y).emplace(z, c);
}

bool World::blockExists(int x, int y, int z) {
    if(chunkExists(floorDiv(x, Chunk::SIZE), floorDiv(y, Chunk::SIZE), floorDiv(z, Chunk::SIZE))) {
        return getChunk(floorDiv(x, Chunk::SIZE), floorDiv(y, Chunk::SIZE), floorDiv(z, Chunk::SIZE))->blockExists(loopMod(x, Chunk::SIZE), loopMod(y, Chunk::SIZE), loopMod(z, Chunk::SIZE));
    }
    return false;
}

Block* World::getBlock(int x, int y, int z) {
    return getChunk(floorDiv(x, Chunk::SIZE), floorDiv(y, Chunk::SIZE), floorDiv(z, Chunk::SIZE))->getBlock(loopMod(x, Chunk::SIZE), loopMod(y, Chunk::SIZE), loopMod(z, Chunk::SIZE));
}

void World::setBlock(int x, int y, int z, Block* b) {
    getChunk(floorDiv(x, Chunk::SIZE), floorDiv(y, Chunk::SIZE), floorDiv(z, Chunk::SIZE))->setBlock(loopMod(x, Chunk::SIZE), loopMod(y, Chunk::SIZE), loopMod(z, Chunk::SIZE), b);
}

void World::draw() {
    TextureMapManager::bindTextureMap("blocks");

    for(const auto& x : chunks) {
        glm::mat4 matX = glm::translate(glm::mat4(1.0f), glm::vec3(x.first * Chunk::SIZE, 0, 0));
        for(const auto& y : chunks.at(x.first)) {
            glm::mat4 matXY = glm::translate(matX, glm::vec3(0, y.first * Chunk::SIZE, 0));
            for(const auto& z : chunks.at(x.first).at(y.first)) {
                if(chunkExists(x.first, y.first, z.first)) {
                    glm::mat4 matXYZ = glm::translate(matXY, glm::vec3(0, 0, z.first * Chunk::SIZE));
                    ShaderProgramManager::setActiveProgram("triangle");
                    ShaderProgramManager::setMat4("model", matXYZ);
                    getChunk(x.first, y.first, z.first)->draw();
                }
            }
        }
    }
}

void swap(World& first, World& second) {
    using std::swap;
    swap(first.chunks, second.chunks);
}

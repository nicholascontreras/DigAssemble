#include "World.h"

#include <stdexcept>

#include <glm/gtc/matrix_transform.hpp>

#include "../managers/ShaderProgramManager.h"
#include "../managers/TextureMapManager.h"

World::World() {
    chunks.emplace(0, std::map<int, std::map<int, Chunk>>());
    chunks.at(0).emplace(0, std::map<int, Chunk>());
    chunks.at(0).at(0).emplace(0, Chunk());
}

World::~World() {
}

World::World(World&& other) noexcept : World() {
    swap(*this, other);
}

bool World::chunkExists(int x, int y, int z) {
    return chunks.count(x) && chunks.at(x).count(y) && chunks.at(x).at(y).count(z);
}

Chunk& World::getChunk(int x, int y, int z) {
    return chunks.at(x).at(y).at(z);
}

void World::draw() {
    TextureMapManager::bindTextureMap("blocks");

    for(int x = 0; x < 4; x++) {
        glm::mat4 matX = glm::translate(glm::mat4(1.0f), glm::vec3(x * Chunk::SIZE, 0, 0));
        for(int y = 0; y < 4; y++) {
            glm::mat4 matXY = glm::translate(matX, glm::vec3(0, y * Chunk::SIZE, 0));
            for(int z = 0; z < 4; z++) {
                if(chunkExists(x, y, z)) {
                    glm::mat4 matXYZ = glm::translate(matXY, glm::vec3(0, 0, z * Chunk::SIZE));
                    ShaderProgramManager::setActiveProgram("triangle");
                    ShaderProgramManager::setMat4("model", matXYZ);
                    getChunk(x, y, z).draw();
                }
            }
        }
    }
}

void swap(World& first, World& second) {
    using std::swap;
    swap(first.chunks, second.chunks);
}

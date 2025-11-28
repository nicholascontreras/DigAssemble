#include "World.h"

#include <stdexcept>
#include <algorithm>

#include <glm/gtc/matrix_transform.hpp>

#include "../managers/ShaderProgramManager.h"
#include "../managers/TextureMapManager.h"
#include "../util/math_utils.h"

World::World(int seed, int spawnY) : seed(seed), spawnY(spawnY) {
}

World::~World() {
    std::unordered_map<Coord, Chunk*> lc = loadedChunks();
    std::for_each(lc.begin(), lc.end(), [](std::pair<Coord, Chunk*> c) { delete c.second; });
}

World::World(World&& other) noexcept : World(other.seed, other.spawnY) {
    swap(*this, other);
}

bool World::chunkExists(int x, int y, int z) const {
    return chunks.count(x) && chunks.at(x).count(y) && chunks.at(x).at(y).count(z);
}

bool World::chunkLoaded(int x, int y, int z) const {
    if(!chunkExists(x, y, z)) {
        throw std::invalid_argument("No chunk!");
    }
    return chunks.at(x).at(y).at(z) != nullptr;
}

Chunk* World::getChunk(int x, int y, int z) const {
    if(!chunkLoaded(x, y, z)) {
        throw std::invalid_argument("Chunk not loaded!");
    }
    return chunks.at(x).at(y).at(z);
}

void World::initChunk(int x, int y, int z, Chunk* c) {
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

void World::loadChunk(int x, int y, int z, Chunk* c) {
    if(chunkLoaded(x, y, z)) {
        throw std::invalid_argument("Chunk already loaded!");
    }

    chunks.at(x).at(y).erase(z);
    chunks.at(x).at(y).emplace(z, c);
}

Chunk* World::unloadChunk(int x, int y, int z) {
    if(!chunkLoaded(x, y, z)) {
        throw std::invalid_argument("Chunk not loaded!");
    }

    Chunk* c = chunks.at(x).at(y).at(z);
    chunks.at(x).at(y).erase(z);
    chunks.at(x).at(y).emplace(z, nullptr);
    return c;
}

Block* World::getBlock(int x, int y, int z) const {
    int cx = Chunk::at(x);
    int cy = Chunk::at(x);
    int cz = Chunk::at(x);

    return getChunk(cx, cy, cz)->getBlock(loopMod(x, Chunk::SIZE), loopMod(y, Chunk::SIZE), loopMod(z, Chunk::SIZE));
}

std::unordered_map<Coord, Chunk*> World::loadedChunks() const {
    std::unordered_map<Coord, Chunk*> lc;
    for(const auto& x : chunks) {
        for(const auto& y : chunks.at(x.first)) {
            for(const auto& z : chunks.at(x.first).at(y.first)) {
                if(z.second != nullptr) {
                    lc.emplace(Coord(x.first, y.first, z.first), z.second);
                }
            }
        }
    }
    return lc;
}

std::unordered_map<Coord, Chunk*> World::allChunks() const {
    std::unordered_map<Coord, Chunk*> ac;
    for(const auto& x : chunks) {
        for(const auto& y : chunks.at(x.first)) {
            for(const auto& z : chunks.at(x.first).at(y.first)) {
                ac.emplace(Coord(x.first, y.first, z.first), z.second);
            }
        }
    }
    return ac;
}

void World::draw() {
    TextureMapManager::bindTextureMap("blocks");

    synchronized(*this) {
        std::unordered_map<Coord, Chunk*> lc = loadedChunks();
        std::for_each(lc.begin(), lc.end(), [&](std::pair<Coord, Chunk*> c) {
            glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(c.first.x * Chunk::SIZE, c.first.y * Chunk::SIZE, c.first.z * Chunk::SIZE));
            ShaderProgramManager::setActiveProgram("triangle");
            ShaderProgramManager::setMat4("model", modelMat);

            c.second->draw();
        });
    }
}

void swap(World& first, World& second) {
    using std::swap;
    swap(first.chunks, second.chunks);
}

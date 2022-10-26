#include "World.h"

#include <stdexcept>

#include <glm/gtc/matrix_transform.hpp>

#include "../ShaderProgramManager.h"

World::World() {
}

World::~World() {
    for(std::pair<int, std::unordered_map<int, std::unordered_map<int, Block*>>> x : blocks) {
        for(std::pair<int, std::unordered_map<int, Block*>> y : x.second) {
            for(std::pair<int, Block*> z : y.second) {
                delete z.second;
            }
        }
    }
}

World::World(World&& other) noexcept : World() {
    swap(*this, other);
}

bool World::isBlock(int x, int y, int z) {
    // unordered_map does not have a "contains key" method but does have a "count key" function
    // because keys must be unique this function either returns 0 or 1 (false or true)
    return blocks.count(x) && blocks.at(x).count(y) && blocks.at(x).at(y).count(z);
}

void World::setBlock(Block* b) {
    if(!blocks.count(b->x)) {
        blocks.emplace(b->x, std::unordered_map<int, std::unordered_map<int, Block*>>());
    }
    if(!blocks.at(b->x).count(b->y)) {
        blocks.at(b->x).emplace(b->y, std::unordered_map<int, Block*>());
    }
    blocks.at(b->x).at(b->y).emplace(b->z, b);
}

Block* World::getBlock(int x, int y, int z) {
    if(isBlock(x, y, z)) {
        return blocks.at(x).at(y).at(z);
    } else {
        return nullptr;
    }
}

void World::draw() {
    for(std::pair<int, std::unordered_map<int, std::unordered_map<int, Block*>>> x : blocks) {
        glm::mat4 matX = glm::translate(glm::mat4(1.0f), glm::vec3(x.first, 0, 0));
        for(std::pair<int, std::unordered_map<int, Block*>> y : x.second) {
            glm::mat4 matXY = glm::translate(matX, glm::vec3(0, y.first, 0));
            for(std::pair<int, Block*> z : y.second) {
                glm::mat4 matXYZ = glm::translate(matXY, glm::vec3(0, 0, z.first));

                ShaderProgramManager::setActiveProgram("triangle");
                ShaderProgramManager::setMat4("model", matXYZ);

                z.second->draw();
            }
        }
    }
}

void swap(World& first, World& second) {
    using std::swap;
    swap(first.blocks, second.blocks);
}

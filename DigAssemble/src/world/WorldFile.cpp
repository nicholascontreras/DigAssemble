#include "WorldFile.h"

#include <filesystem>
#include <fstream>

#include "../util/str_utils.h"

void WorldFile::saveWorld(const World& world, const std::string& saveFolderPath) {
    std::filesystem::create_directory(saveFolderPath);

    std::ofstream metadataFile = std::ofstream(saveFolderPath + "/data");
    if(!metadataFile.is_open()) {
        throw std::runtime_error("Failed to open file!");
    }

    std::string metadataString = std::to_string(world.seed) + "," + std::to_string(world.spawnY);
    metadataFile.write(metadataString.c_str(), metadataString.length());
    metadataFile.close();

    Async::lock(world);
    std::ofstream chunkListFile = std::ofstream(saveFolderPath + "/chunkList");
    if(!chunkListFile.is_open()) {
        throw std::runtime_error("Failed to open file!");
    }

    for(std::pair<Coord, Chunk*> cur : world.allChunks()) {
        std::string s = std::to_string(cur.first.x) + "," + std::to_string(cur.first.y) + "," + std::to_string(cur.first.z) + ";";
        chunkListFile.write(s.c_str(), s.length());
    }
    chunkListFile.close();

    std::filesystem::create_directory(saveFolderPath + "/chunks");
    for(std::pair<Coord, Chunk*> cur : world.loadedChunks()) {
        saveChunk(cur.second, saveFolderPath, cur.first.x, cur.first.y, cur.first.z);
    }
    Async::unlock(world);
}

void WorldFile::saveChunk(const Chunk* chunk, const std::string& saveFolderPath, int cx, int cy, int cz) {
    std::string chunkFileName = std::to_string(cx) + " " + std::to_string(cy) + " " + std::to_string(cz);
    std::ofstream chunkFile = std::ofstream(saveFolderPath + "/chunks/" + chunkFileName);
    if(!chunkFile.is_open()) {
        throw std::runtime_error("Failed to open file!");
    }

    for(int x = 0; x < Chunk::SIZE; x++) {
        for(int y = 0; y < Chunk::SIZE; y++) {
            for(int z = 0; z < Chunk::SIZE; z++) {
                if(chunk->blockExists(x, y, z)) {
                    std::string s = chunk->getBlock(x, y, z)->getTextureName() + ",";
                    chunkFile.write(s.c_str(), s.length());
                } else {
                    chunkFile.write(",", 1);
                }
            }
        }
    }

    chunkFile.close();
}

World WorldFile::loadWorld(const std::string& saveFolderPath) {
    std::ifstream metadataFile = std::ifstream(saveFolderPath + "/data");
    if(!metadataFile.is_open()) {
        throw std::runtime_error("Failed to open file!");
    }

    std::string metadataString;
    std::getline(metadataFile, metadataString);
    metadataFile.close();
    std::vector<std::string> splitMetadata = split(metadataString, ",");

    World world = World(std::stoi(splitMetadata.at(0)), std::stoi(splitMetadata.at(1)));

    std::ifstream chunkListFile = std::ifstream(saveFolderPath + "/chunkList");
    if(!chunkListFile.is_open()) {
        throw std::runtime_error("Failed to open file!");
    }

    std::string chunkListString;
    std::getline(chunkListFile, chunkListString);
    chunkListFile.close();
    std::vector<std::string> splitChunkList = split(chunkListString, ";");

    for(std::string curChunkString : splitChunkList) {
        std::vector<std::string> splitChunkCoords = split(curChunkString, ",");
        int cx = std::stoi(splitChunkCoords.at(0));
        int cy = std::stoi(splitChunkCoords.at(1));
        int cz = std::stoi(splitChunkCoords.at(2));
        world.initChunk(cx, cy, cz, nullptr);
    }

    return world;
}

Chunk* WorldFile::loadChunk(const std::string& saveFolderPath, int cx, int cy, int cz) {
    std::string chunkFileName = std::to_string(cx) + " " + std::to_string(cy) + " " + std::to_string(cz);
    std::ifstream chunkFile = std::ifstream(saveFolderPath + "/chunks/" + chunkFileName);
    if(!chunkFile.is_open()) {
        throw std::runtime_error("Failed to open file!");
    }

    std::string chunkString;
    std::getline(chunkFile, chunkString);
    chunkFile.close();
    std::vector<std::string> splitChunkString = split(chunkString, ",");

    if(splitChunkString.size() != Chunk::SIZE * Chunk::SIZE * Chunk::SIZE) {
        throw std::runtime_error("Invalid chunk read!");
    }

    Chunk* c = new Chunk();

    int blockIndex = 0;
    for(int x = 0; x < Chunk::SIZE; x++) {
        for(int y = 0; y < Chunk::SIZE; y++) {
            for(int z = 0; z < Chunk::SIZE; z++) {
                std::string curBlockString = splitChunkString.at(blockIndex);
                if(!curBlockString.empty()) {
                    c->setBlock(x, y, z, new Block(curBlockString));
                }
                blockIndex++;
            }
        }
    }

    return c;
}

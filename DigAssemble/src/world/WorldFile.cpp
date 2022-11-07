#include "WorldFile.h"

#include <filesystem>
#include <fstream>

#include "../util/str_utils.h"

void WorldFile::saveWorld(const World& world, const std::string& fileName) {
    std::string savePath = "C:/Users/rf741f/Desktop/saves/" + fileName;
    std::filesystem::create_directory(savePath);

    std::ofstream metadataFile = std::ofstream(savePath + "/data");
    std::string metadataString = std::to_string(world.seed) + "," + std::to_string(world.spawnY);
    metadataFile.write(metadataString.c_str(), metadataString.length());
    metadataFile.close();

    std::ofstream chunkListFile = std::ofstream(savePath + "/chunkList");
    for(std::pair<Coord, Chunk*> cur : world.allChunks()) {
        std::string s = std::to_string(cur.first.x) + "," + std::to_string(cur.first.y) + "," + std::to_string(cur.first.z) + ";";
        chunkListFile.write(s.c_str(), s.length());
    }
    chunkListFile.close();

    std::filesystem::create_directory(savePath + "/chunks");
    for(std::pair<Coord, Chunk*> cur : world.allChunks()) {
        std::string s = std::to_string(cur.first.x) + " " + std::to_string(cur.first.y) + " " + std::to_string(cur.first.z);
        saveChunk(cur.second, savePath + "/chunks/" + s);
    }
}

void WorldFile::saveChunk(const Chunk* chunk, const std::string& fileName) {
    std::ofstream chunkFile = std::ofstream(fileName);

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

World WorldFile::loadWorld(const std::string& fileName) {
    std::string savePath = "C:/Users/rf741f/Desktop/saves/" + fileName;

    std::ifstream metadataFile = std::ifstream(savePath + "/data");
    std::string metadataString;
    std::getline(metadataFile, metadataString);
    metadataFile.close();
    std::vector<std::string> splitMetadata = split(metadataString, ",");

    World world = World(std::stoi(splitMetadata.at(0)), std::stoi(splitMetadata.at(1)));

    std::ifstream chunkListFile = std::ifstream(savePath + "/chunkList");
    std::string chunkListString;
    std::getline(chunkListFile, chunkListString);
    chunkListFile.close();
    std::vector<std::string> splitChunkList = split(chunkListString, ";");

    for(std::string curChunkString : splitChunkList) {
        std::vector<std::string> splitChunkCoords = split(curChunkString, ",");
        int cx = std::stoi(splitChunkCoords.at(0));
        int cy = std::stoi(splitChunkCoords.at(1));
        int cz = std::stoi(splitChunkCoords.at(2));
        world.initChunk(cx, cy, cz);
    }

    return world;
}

Chunk* WorldFile::loadChunk(const std::string& fileName, int cx, int cy, int cz) {
    std::string chunkFileName = std::to_string(cx) + " " + std::to_string(cy) + " " + std::to_string(cz);
    std::ifstream chunkFile = std::ifstream(fileName + "/chunks/" + chunkFileName);
    std::string chunkString;
    std::getline(chunkFile, chunkString);
    chunkFile.close();
    std::vector<std::string> splitChunkString = split(chunkString, ",");

    Chunk* c = new Chunk();

    int blockIndex = 0;
    for(int x = 0; x < Chunk::SIZE; x++) {
        for(int y = 0; y < Chunk::SIZE; y++) {
            for(int z = 0; z < Chunk::SIZE; z++) {
                std::string curBlockString = splitChunkString.at(blockIndex);
                if(!curBlockString.empty()) {
                    c->setBlock(x, y, z, new Block(curBlockString));
                }
            }
        }
    }

    return c;
}

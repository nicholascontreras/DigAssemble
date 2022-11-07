#include "WorldStreaming.h"

#include "worldgen/WorldGen.h"
#include "WorldFile.h"
#include "../util/async/Async.h"

int WorldStreaming::RENDER_DISTANCE = -1;
std::string WorldStreaming::FILE_NAME;

void WorldStreaming::start(World& world, const Player& player) {
    Async::startThread([&] {
        int cx, cy, cz;
        if(needLoadChunk(world, player, cx, cy, cz)) {
            createOrStreamChunk(world, cx, cy, cz);
        }

        unloadChunkIfPossible(world, player);

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    });
}

bool WorldStreaming::needLoadChunk(const World& world, const Player& player, int& cx, int& cy, int& cz) {

    const int playerChunkX = Chunk::at(player.getX());
    const int playerChunkY = Chunk::at(player.getY());
    const int playerChunkZ = Chunk::at(player.getZ());

    for(int x = playerChunkX - RENDER_DISTANCE; x <= playerChunkX + RENDER_DISTANCE; x++) {
        for(int y = playerChunkY - RENDER_DISTANCE; y <= playerChunkY + RENDER_DISTANCE; y++) {
            for(int z = playerChunkZ - RENDER_DISTANCE; z <= playerChunkZ + RENDER_DISTANCE; z++) {
                float distance = sqrtf(powf((float)x - playerChunkX, 2) + powf((float)y - playerChunkY, 2) + powf((float)z - playerChunkZ, 2));
                if(distance <= RENDER_DISTANCE) {
                    if(!world.chunkExists(x, y, z) || !world.chunkLoaded(x, y, z)) {
                        cx = x;
                        cy = y;
                        cz = z;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void WorldStreaming::createOrStreamChunk(World& world, int cx, int cy, int cz) {
    if(!world.chunkExists(cx, cy, cz)) {
        WorldGen::generateChunk(world, cx, cy, cz);
    } else {
        std::string chunkFileName = std::to_string(cx) + " " + std::to_string(cy) + " " + std::to_string(cz);
        Chunk* c = WorldFile::loadChunk(FILE_NAME + "/chunks/" + chunkFileName, cx, cy, cz);
        world.loadChunk(cx, cy, cz, c);
    }
}

void WorldStreaming::unloadChunkIfPossible(World& world, const Player& player) {
    const int playerChunkX = Chunk::at(player.getX());
    const int playerChunkY = Chunk::at(player.getY());
    const int playerChunkZ = Chunk::at(player.getZ());

    for(std::pair<Coord, Chunk*> cur : world.loadedChunks()) {
        int cx = cur.first.x;
        int cy = cur.first.y;
        int cz = cur.first.z;

        float distance = sqrtf(powf((float)cx - playerChunkX, 2) + powf((float)cy - playerChunkY, 2) + powf((float)cz - playerChunkZ, 2));
        if(distance > RENDER_DISTANCE + 1) {
            std::string chunkFileName = std::to_string(cx) + " " + std::to_string(cy) + " " + std::to_string(cz);
            Chunk* c = world.unloadChunk(cx, cy, cz);
            WorldFile::saveChunk(c, chunkFileName);
            delete c;
            return;
        }
    }
}
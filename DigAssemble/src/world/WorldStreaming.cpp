#include "WorldStreaming.h"

#include "worldgen/WorldGen.h"
#include "WorldFile.h"
#include "../util/async/Async.h"
#include "../util/async/AsyncWorker.h"
#include "../util/debug.h"

int WorldStreaming::CHUNK_LOAD_DISTANCE = -1;
std::string WorldStreaming::SAVE_FOLDER_PATH;

void WorldStreaming::start(World& world, const Player& player) {
    WorldGen::setSeed(world.seed);
    WorldFile::saveWorld(world, SAVE_FOLDER_PATH);

    Async::startThread([&] {
        while(Async::runThread()) {
            int cx, cy, cz;
            if(needLoadChunk(world, player, cx, cy, cz)) {
                createOrStreamChunk(world, cx, cy, cz);
            }


            unloadChunkIfPossible(world, player);

            std::this_thread::yield();
        }
    });
}

bool WorldStreaming::needLoadChunk(const World& world, const Player& player, int& cx, int& cy, int& cz) {

    const int playerChunkX = Chunk::at(player.getX());
    const int playerChunkY = Chunk::at(player.getY());
    const int playerChunkZ = Chunk::at(player.getZ());

    float lowestDistance = FLT_MAX;
    bool needLoad = false;

    // Check all the chunks within the cube (taxicab) distance of CHUNK_LOAD_DISTANCE from playerChunk
    for(int x = playerChunkX - CHUNK_LOAD_DISTANCE; x <= playerChunkX + CHUNK_LOAD_DISTANCE; x++) {
        for(int y = playerChunkY - CHUNK_LOAD_DISTANCE; y <= playerChunkY + CHUNK_LOAD_DISTANCE; y++) {
            for(int z = playerChunkZ - CHUNK_LOAD_DISTANCE; z <= playerChunkZ + CHUNK_LOAD_DISTANCE; z++) {

                // For each chunk determine if it is actually within the CHUNK_LOAD_DISTANCE
                float distance = sqrtf(powf((float)x - playerChunkX, 2) + powf((float)y - playerChunkY, 2) + powf((float)z - playerChunkZ, 2));
                if (distance <= CHUNK_LOAD_DISTANCE) {

                    // If the chunk is in range determine if we need to load it
                    if (!world.chunkExists(x, y, z) || !world.chunkLoaded(x, y, z)) {
                        needLoad = true;

                        // Only save off the chunk if it's the closest one we've found so far
                        if (distance < lowestDistance) {
                            lowestDistance = distance;
                            cx = x;
                            cy = y;
                            cz = z;
                        }
                    }
                }
            }
        }
    }

    return needLoad;
}

void WorldStreaming::createOrStreamChunk(World& world, int cx, int cy, int cz) {
    if(!world.chunkExists(cx, cy, cz)) {
        WorldGen::generateChunk(world, cx, cy, cz);
    } else {
        Chunk* c = WorldFile::loadChunk(SAVE_FOLDER_PATH, cx, cy, cz);
        synchronized(world) {
            world.loadChunk(cx, cy, cz, c);
        }
    }

    int geometryConstructionBufferSizeUsed = world.getChunk(cx, cy, cz)->constructLocalGeometry();
    AsyncWorker::runOnMain([&world, cx, cy, cz, geometryConstructionBufferSizeUsed] {
        world.getChunk(cx, cy, cz)->sendGeometryToGraphics(geometryConstructionBufferSizeUsed);
        Async::unlock("chunkToGraphics");
    });
    Async::lock("chunkToGraphics");
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
        if(distance > CHUNK_LOAD_DISTANCE + 1) {
            
            Chunk* c = world.unloadChunk(cx, cy, cz);
            WorldFile::saveChunk(c, SAVE_FOLDER_PATH, cx, cy, cz);

            synchronized(world) {
                delete c;
            }

            return;
        }
    }
}
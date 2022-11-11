#include "WorldStreaming.h"

#include "worldgen/WorldGen.h"
#include "WorldFile.h"
#include "../util/async/Async.h"
#include "../util/async/AsyncWorker.h"
#include "../util/debug.h"

int WorldStreaming::RENDER_DISTANCE = -1;
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

    for(int a = 0; a <= RENDER_DISTANCE * 2; a++) {
        for(int b = 0; b <= RENDER_DISTANCE * 2; b++) {
            for(int c = 0; c <= RENDER_DISTANCE * 2; c++) {
                int x = ((int)ceil(a / 2.0f) * (a % 2 == 0 ? 1 : -1)) + playerChunkX;
                int y = ((int)ceil(b / 2.0f) * (b % 2 == 0 ? 1 : -1)) + playerChunkY;
                int z = ((int)ceil(c / 2.0f) * (c % 2 == 0 ? 1 : -1)) + playerChunkZ;

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
        if(distance > RENDER_DISTANCE + 1) {
            
            Chunk* c = world.unloadChunk(cx, cy, cz);
            WorldFile::saveChunk(c, SAVE_FOLDER_PATH, cx, cy, cz);

            synchronized(world) {
                delete c;
            }

            return;
        }
    }
}
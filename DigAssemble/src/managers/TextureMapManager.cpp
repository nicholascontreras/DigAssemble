#include "TextureMapManager.h"

#include <filesystem>

#include <stb_image/stb_image.h>
#include <algorithm> 
#include <stdexcept>
#include "TextureManager.h"

const std::string TextureMapManager::TEXTURE_NAME_PREFIX = "TextureMap_";

std::unordered_map<std::string, TextureMapManager::TextureMapData> TextureMapManager::textureMaps;

void TextureMapManager::generateTextureMap(const std::string& textureMapName) {
    std::string textureFolderPath = "textures/" + textureMapName;

    TextureMapData textureMapData;
    std::vector<std::vector<glm::vec4>> textureMap;

    stbi_set_flip_vertically_on_load(true);
    for(const std::filesystem::directory_entry& curFile : std::filesystem::directory_iterator(textureFolderPath)) {
        std::string filePath = curFile.path().string();
        std::string fileName = curFile.path().stem().string();

        int width = -1, height = -1, numChannels = -1;
        unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &numChannels, 0);

        if(numChannels != 4) {
            throw std::runtime_error("TextureMap images must be RGBA!");
        }

        if(width > textureMapData.width) {
            expandWidth(textureMap, textureMapData, width);
        }

        expandHeight(textureMap, textureMapData, textureMapData.height + height);

        int startingRow = (int) textureMap.size() - height;
        for(int y = 0; y < height; y++) {
            std::vector<glm::vec4>& curRow = textureMap.at((size_t)startingRow + y);
            for(int x = 0; x < width; x++) {
                int dataPos = (y * width * 4) + (x * 4);
                curRow.at(x) = glm::vec4(data[dataPos], data[dataPos + 1], data[dataPos + 2], data[dataPos + 3]);
            }
        }

        stbi_image_free(data);
        textureMapData.textureLocations.emplace(fileName, glm::vec3(startingRow, width, height));
    }

    unsigned char* textureMapBytes = flattenTextureMap(textureMap);
    TextureManager::generateTexture(TEXTURE_NAME_PREFIX + textureMapName, textureMapData.width, textureMapData.height, 4, textureMapBytes);
    delete[] textureMapBytes;
    
    textureMaps.emplace(textureMapName, textureMapData);
}

glm::vec4 TextureMapManager::getTextureCoords(const std::string& texureMapName, const std::string& textureName) {
    const TextureMapData& textureMapData = textureMaps.at(texureMapName);
    const glm::vec3& textureBox = textureMapData.textureLocations.at(textureName);
    return glm::vec4(0.0f, 
                    (float) textureBox[0] / textureMapData.height, 
                    (float) textureBox[1] / textureMapData.width, 
                    ((float) textureBox[0] + textureBox[2]) / textureMapData.height);
}

void TextureMapManager::bindTextureMap(const std::string textureMapName) {
    TextureManager::bindTexture(TEXTURE_NAME_PREFIX + textureMapName);
}

void TextureMapManager::expandWidth(std::vector<std::vector<glm::vec4>>& textureMap, TextureMapData& textureMapData, unsigned int newWidth) {
    for(std::vector<glm::vec4>& curRow : textureMap) {
        curRow.resize(newWidth, glm::vec4(1));
    }
    textureMapData.width = newWidth;
}

void TextureMapManager::expandHeight(std::vector<std::vector<glm::vec4>>& textureMap, TextureMapData& textureMapData, unsigned int newHeight) {
    textureMap.resize(newHeight, std::vector<glm::vec4>(textureMapData.width, glm::vec4(1)));
    textureMapData.height = newHeight;
}

unsigned char* TextureMapManager::flattenTextureMap(const std::vector<std::vector<glm::vec4>>& textureMap) {
    if(textureMap.empty()) {
        throw std::invalid_argument("What's 9 + 10? 21. You stupid.");
    }
    
    int width = (int) textureMap.at(0).size();
    int flattenedLength = (int) textureMap.size() * width * 4;
    unsigned char* flattened = new unsigned char[flattenedLength];

    for(int i = 0; i < flattenedLength; i++) {
        int x = (i / 4) % width;
        int y = (i / 4) / width;
        flattened[i] = (unsigned char) textureMap.at(y).at(x)[i % 4];
    }

    return flattened;
}

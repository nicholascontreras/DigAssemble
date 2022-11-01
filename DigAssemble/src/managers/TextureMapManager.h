#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>

class TextureMapManager {
public:
    static void generateTextureMap(const std::string& textureMapName);

    static glm::vec4 getTextureCoords(const std::string& texureMapName, const std::string& textureName);
    static void bindTextureMap(const std::string textureMapName);

private:
    static const std::string TEXTURE_NAME_PREFIX;

    struct TextureMapData {
        int width = 0, height = 0;
        std::unordered_map<std::string, glm::vec3> textureLocations;
    };

    static std::unordered_map<std::string, TextureMapData> textureMaps;

    static void expandWidth(std::vector<std::vector<glm::vec4>>& textureMap, TextureMapData& textureMapData, unsigned int newWidth);
    static void expandHeight(std::vector<std::vector<glm::vec4>>& textureMap, TextureMapData& textureMapData, unsigned int newHeight);

    static unsigned char* flattenTextureMap(const std::vector<std::vector<glm::vec4>>& textureMap);
};


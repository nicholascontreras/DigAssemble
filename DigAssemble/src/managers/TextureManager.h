#pragma once

#include <string>
#include <unordered_map>

class TextureManager {
public:
    static void generateTexture(const std::string& textureName);
    static void generateTexture(const std::string& textureName, unsigned int width, unsigned int height, unsigned int channels, const unsigned char* textureData);

    static void bindTexture(const std::string& textureName);
private:
    static std::unordered_map<std::string, unsigned int> textures;
    static std::string activeTexture;

    static void addTextureToMap(const std::string& textureName, unsigned int textureId);
};


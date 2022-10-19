#pragma once

#include <string>
#include <unordered_map>

class TextureManager {
public:
    static void generateTexture(const std::string& texture);
    static void bindTexture(const std::string& texture);

private:
    static std::unordered_map<std::string, unsigned int> textures;
    static std::string activeTexture;
};


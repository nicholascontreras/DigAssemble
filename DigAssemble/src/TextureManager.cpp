#include "TextureManager.h"

#include "stb_image.h"

#include "glad/glad.h"

std::unordered_map<std::string, unsigned int> TextureManager::textures;
std::string TextureManager::activeTexture;

void TextureManager::generateTexture(const std::string& textureName) {
    std::string textureFileName = "textures/" + textureName + ".png";

    int width = -1, height = -1, numChannels = -1;
    unsigned char* data = stbi_load(textureFileName.c_str(), &width, &height, &numChannels, 0);

    unsigned int textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    activeTexture = textureName;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);

    textures.emplace(textureName, textureId);
}

void TextureManager::bindTexture(const std::string& textureName) {
    if(textureName != activeTexture) {
        glBindTexture(GL_TEXTURE_2D, textures.at(textureName));
        activeTexture = textureName;
    }
}

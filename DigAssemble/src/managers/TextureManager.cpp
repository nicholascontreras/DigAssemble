#include "TextureManager.h"

#include <stdexcept>

#include <stb_image/stb_image.h>
#include <glad/gl.h>

std::unordered_map<std::string, unsigned int> TextureManager::textures;
std::string TextureManager::activeTexture;

void TextureManager::generateTexture(const std::string& textureName) {
    std::string textureFileName = "textures/" + textureName + ".png";

    stbi_set_flip_vertically_on_load(true);

    int width = -1, height = -1, numChannels = -1;
    unsigned char* data = stbi_load(textureFileName.c_str(), &width, &height, &numChannels, 0);

    if(numChannels != 4) {
        throw std::runtime_error("Texture images must be RGBA!");
    }

    unsigned int textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    activeTexture = textureName;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);

    addTextureToMap(textureName, textureId);
}

void TextureManager::generateTexture(const std::string& textureName, unsigned int width, unsigned int height, unsigned int channels, const unsigned char* textureData) {
    unsigned int imgType = 0;
    switch(channels) {
    case 1:
        imgType = GL_RED;
        break;
    case 4:
        imgType = GL_RGBA;
        break;
    default:
        throw std::invalid_argument("Invalid number of channels!");
    }
    
    
    unsigned int textureId = 0;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    activeTexture = textureName;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, imgType, width, height, 0, imgType, GL_UNSIGNED_BYTE, textureData);

    addTextureToMap(textureName, textureId);
}

void TextureManager::bindTexture(const std::string& textureName) {
    if(textureName != activeTexture) {
        glBindTexture(GL_TEXTURE_2D, textures.at(textureName));
        activeTexture = textureName;
    }
}

void TextureManager::addTextureToMap(const std::string& textureName, unsigned int textureId) {
    if(textures.count(textureName)) {
        throw std::invalid_argument("The given name is already in use!");
    } else {
        textures.emplace(textureName, textureId);
    }
}

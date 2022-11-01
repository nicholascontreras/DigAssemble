#include "Block.h"

#include "../../managers/TextureMapManager.h"

const float Block::VERTICES[] = {
    // Front face
     0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
     1.0f,  0.0f,  1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
     0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
     1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
     0.0f,  1.0f,  1.0f,  0.0f, 1.0f,

     // Back face
     0.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     1.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     1.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     // Left face
     0.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
     0.0f,  1.0f,  1.0f,  1.0f, 1.0f,
     0.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.0f,  1.0f,  1.0f,  1.0f, 1.0f,
     0.0f,  1.0f,  0.0f,  0.0f, 1.0f,

     // Right face
     1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
     1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
     1.0f,  0.0f,  1.0f,  1.0f, 0.0f,

     // Top face
     0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
     0.0f,  1.0f,  1.0f,  0.0f, 1.0f,
     1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
     0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
     1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
     1.0f,  1.0f,  0.0f,  1.0f, 0.0f,

     // Bottom face
     0.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     1.0f,  0.0f,  1.0f,  1.0f, 1.0f,
     0.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     1.0f,  0.0f,  1.0f,  1.0f, 1.0f,
     0.0f,  0.0f,  1.0f,  0.0f, 1.0f
};

unsigned int Block::getNumFaceVertices() {
    return NUM_VERTICES / 6;
}

Block::Block(const std::string& textureName) : textureName(textureName) {
}

void Block::appendPosXFace(float* geometry, unsigned int& geometrySizeUsed, const glm::mat4& translation) {
    appendFace(geometry, geometrySizeUsed, translation, 3 * VERTICES_PER_FACE * ELEMENTS_PER_VERTEX);
}

void Block::appendNegXFace(float* geometry, unsigned int& geometrySizeUsed, const glm::mat4& translation) {
    appendFace(geometry, geometrySizeUsed, translation, 2 * VERTICES_PER_FACE * ELEMENTS_PER_VERTEX);
}

void Block::appendPosYFace(float* geometry, unsigned int& geometrySizeUsed, const glm::mat4& translation) {
    appendFace(geometry, geometrySizeUsed, translation, 4 * VERTICES_PER_FACE * ELEMENTS_PER_VERTEX);
}

void Block::appendNegYFace(float* geometry, unsigned int& geometrySizeUsed, const glm::mat4& translation) {
    appendFace(geometry, geometrySizeUsed, translation, 5 * VERTICES_PER_FACE * ELEMENTS_PER_VERTEX);
}

void Block::appendPosZFace(float* geometry, unsigned int& geometrySizeUsed, const glm::mat4& translation) {
    appendFace(geometry, geometrySizeUsed, translation, 0 * VERTICES_PER_FACE * ELEMENTS_PER_VERTEX);
}

void Block::appendNegZFace(float* geometry, unsigned int& geometrySizeUsed, const glm::mat4& translation) {
    appendFace(geometry, geometrySizeUsed, translation, 1 * VERTICES_PER_FACE * ELEMENTS_PER_VERTEX);
}

void Block::appendFace(float* geometry, unsigned int& geometrySizeUsed, const glm::mat4& translation, unsigned int offset) {
    
    glm::vec4 textureBox = TextureMapManager::getTextureCoords("blocks", textureName);

    for(int i = 0; i < VERTICES_PER_FACE; i++) {
        glm::vec3 translatedCoords = translation * glm::vec4(VERTICES[offset], VERTICES[offset + 1], VERTICES[offset + 2], 1);
        float texU = (VERTICES[offset + 3] * (textureBox[2] - textureBox[0])) + textureBox[0];
        float texV = (VERTICES[offset + 4] * (textureBox[3] - textureBox[1])) + textureBox[1];

        offset += 5;

        geometry[geometrySizeUsed++] = translatedCoords.x;
        geometry[geometrySizeUsed++] = translatedCoords.y;
        geometry[geometrySizeUsed++] = translatedCoords.z;
        geometry[geometrySizeUsed++] = texU;
        geometry[geometrySizeUsed++] = texV;
    }
}

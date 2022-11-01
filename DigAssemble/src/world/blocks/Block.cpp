#include "Block.h"

#include "../../managers/TextureMapManager.h"

// Define fairly precise fractions for extracting textures for each face
const float Block::H0 = 0, Block::H1 = 1 / 4.0f, Block::H2 = 2 / 4.0f, Block::H3 = 3 / 4.0f, Block::H4 = 4 / 4.0f;
const float Block::V0 = 0, Block::V1 = 1 / 3.0f, Block::V2 = 2 / 3.0f, Block::V3 = 3 / 3.0f;

const float Block::VERTICES[] = {
    // Front face
     0.0f,  0.0f,  1.0f,  H1, V1,
     1.0f,  0.0f,  1.0f,  H2, V1,
     1.0f,  1.0f,  1.0f,  H2, V2,
     0.0f,  0.0f,  1.0f,  H1, V1,
     1.0f,  1.0f,  1.0f,  H2, V2,
     0.0f,  1.0f,  1.0f,  H1, V2,

     // Back face
     0.0f,  0.0f,  0.0f,  H4, V1,
     0.0f,  1.0f,  0.0f,  H4, V2,
     1.0f,  1.0f,  0.0f,  H3, V2,
     0.0f,  0.0f,  0.0f,  H4, V1,
     1.0f,  1.0f,  0.0f,  H3, V2,
     1.0f,  0.0f,  0.0f,  H3, V1,

     // Left face
     0.0f,  0.0f,  0.0f,  H0, V1,
     0.0f,  0.0f,  1.0f,  H1, V1,
     0.0f,  1.0f,  1.0f,  H1, V2,
     0.0f,  0.0f,  0.0f,  H0, V1,
     0.0f,  1.0f,  1.0f,  H1, V2,
     0.0f,  1.0f,  0.0f,  H0, V2,

     // Right face
     1.0f,  0.0f,  0.0f,  H3, V1,
     1.0f,  1.0f,  0.0f,  H3, V2,
     1.0f,  1.0f,  1.0f,  H2, V2,
     1.0f,  0.0f,  0.0f,  H3, V1,
     1.0f,  1.0f,  1.0f,  H2, V2,
     1.0f,  0.0f,  1.0f,  H2, V1,

     // Top face
     0.0f,  1.0f,  0.0f,  H1, V3,
     0.0f,  1.0f,  1.0f,  H1, V2,
     1.0f,  1.0f,  1.0f,  H2, V2,
     0.0f,  1.0f,  0.0f,  H1, V3,
     1.0f,  1.0f,  1.0f,  H2, V2,
     1.0f,  1.0f,  0.0f,  H2, V3,

     // Bottom face
     0.0f,  0.0f,  0.0f,  H1, V0,
     1.0f,  0.0f,  0.0f,  H2, V0,
     1.0f,  0.0f,  1.0f,  H2, V1,
     0.0f,  0.0f,  0.0f,  H1, V0,
     1.0f,  0.0f,  1.0f,  H2, V1,
     0.0f,  0.0f,  1.0f,  H1, V1
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

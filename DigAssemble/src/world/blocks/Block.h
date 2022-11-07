#pragma once

#include <string>
#include <glm/glm.hpp>

class Block {
public:
    static const unsigned int NUM_VERTICES = 36, VERTICES_PER_FACE = 6, ELEMENTS_PER_VERTEX = 5;
    
    static unsigned int getNumFaceVertices();

    Block(const std::string& textureName);

    std::string getTextureName();

    void appendPosXFace(float* geometry, unsigned int& geometrySizeUsed, const glm::mat4& translation);
    void appendNegXFace(float* geometry, unsigned int& geometrySizeUsed, const glm::mat4& translation);
    void appendPosYFace(float* geometry, unsigned int& geometrySizeUsed, const glm::mat4& translation);
    void appendNegYFace(float* geometry, unsigned int& geometrySizeUsed, const glm::mat4& translation);
    void appendPosZFace(float* geometry, unsigned int& geometrySizeUsed, const glm::mat4& translation);
    void appendNegZFace(float* geometry, unsigned int& geometrySizeUsed, const glm::mat4& translation);
private:
    static const float H0, H1, H2, H3, H4, V0, V1, V2, V3;
    static const float VERTICES[];

    const std::string textureName;

    void appendFace(float* geometry, unsigned int& geometrySizeUsed, const glm::mat4& translation, unsigned int offset);
};


#include "Block.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

#include "../../TextureManager.h"

unsigned int Block::VAO = 0;
unsigned int Block::VBO = 0;

void Block::init() {
    float vertices[] = {
         0.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         1.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.0f,  0.0f,  0.0f,  0.0f, 0.0f,

         0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
         1.0f,  0.0f,  1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
         1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
         0.0f,  1.0f,  1.0f,  0.0f, 1.0f,
         0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

         0.0f,  1.0f,  1.0f,  1.0f, 0.0f,
         0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
         0.0f,  1.0f,  1.0f,  1.0f, 0.0f,

         1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         1.0f,  0.0f,  1.0f,  0.0f, 0.0f,
         1.0f,  1.0f,  1.0f,  1.0f, 0.0f,

         0.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         1.0f,  0.0f,  1.0f,  1.0f, 0.0f,
         1.0f,  0.0f,  1.0f,  1.0f, 0.0f,
         0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
         0.0f,  0.0f,  0.0f,  0.0f, 1.0f,

         0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         1.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
         0.0f,  1.0f,  1.0f,  0.0f, 0.0f,
         0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36 * 5, vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

Block::Block(int x, int y, int z) : x(x), y(y), z(z) {
}

void Block::draw() const {
    TextureManager::bindTexture("container");

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
};
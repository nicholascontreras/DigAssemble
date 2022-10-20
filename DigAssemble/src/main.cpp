#include <iostream>
#include <cassert>

#include "util/debug.h"
#include "Camera.h"

#include <glad/glad.h> 
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ShaderProgramManager.h"
#include "TextureManager.h"
#include "UIText.h"
#include "blocks/Block.h"

void GLAPIENTRY handleError(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam) {
    throw message;
};

void framebufferSizeChanged(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void assertTypes() {
    assert(sizeof(GLenum) == sizeof(unsigned int));
    assert(sizeof(GLint) == sizeof(int));
    assert(sizeof(GLuint) == sizeof(unsigned int));
    assert(sizeof(GLsizei) == sizeof(int));
    assert(sizeof(GLchar) == sizeof(char));
    assert(sizeof(GLubyte) == sizeof(unsigned char));

    assert(GL_FALSE == false);
    assert(GL_TRUE == true);
}

int main(int argc, char** argv) {
    glfwInit();

    assertTypes();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Window Title", nullptr, nullptr);
    if (window == nullptr) {
        Debug("Failed to create GLFW window");
        return -1;
    }
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        Debug("Failed to initialize GLAD");
        return -1;
    }

    Debug(glGetString(GL_VERSION));

    glEnable(GL_DEPTH_TEST);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(handleError, nullptr);

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebufferSizeChanged);

    ShaderProgramManager::compileProgram("triangle");
    TextureManager::generateTexture("container");

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, Block::numVertices * sizeof(float) * 5, Block::vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    Block block(0, 0, 0);
    Camera camera;

    UIText::init();

    ShaderProgramManager::setActiveProgram("triangle");
    ShaderProgramManager::setMat4("projection", camera.getProjectionMat());

    TextureManager::bindTexture("container");

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        ShaderProgramManager::setMat4("view", camera.getViewMat());
        ShaderProgramManager::setMat4("model", block.getModelMat());  

        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, Block::numVertices);

        UIText::drawText("hello", 100, 100)

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
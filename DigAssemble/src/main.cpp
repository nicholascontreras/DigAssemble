#include <iostream>
#include <cassert>
#include <stdexcept>
#include <Windows.h>

#include "util/debug.h"
#include "Camera.h"

#include <glad/glad.h> 
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ShaderProgramManager.h"
#include "TextureManager.h"
#include "UIText.h"
#include "world/World.h"
#include "world/blocks/Block.h"

Camera camera(1);

void GLAPIENTRY handleError(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam) {
    UNREFERENCED_PARAMETER(source);
    UNREFERENCED_PARAMETER(type);
    UNREFERENCED_PARAMETER(id);
    UNREFERENCED_PARAMETER(severity);
    UNREFERENCED_PARAMETER(length);
    UNREFERENCED_PARAMETER(userParam);

    throw std::runtime_error(message);
};

void framebufferSizeChanged(GLFWwindow* window, int width, int height) {
    UNREFERENCED_PARAMETER(window);

    glViewport(0, 0, width, height);
    camera.setAspectRatio((float) width / height);
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
    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    glfwInit();

    assertTypes();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    GLFWwindow* window = glfwCreateWindow(600, 600, "Window Title", nullptr, nullptr);
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

    glViewport(0, 0, 600, 600);
    glfwSetFramebufferSizeCallback(window, framebufferSizeChanged);

    ShaderProgramManager::compileProgram("triangle");
    ShaderProgramManager::compileProgram("text");
    TextureManager::generateTexture("container");

    Block::init();

    World world;
    world.setBlock(new Block(0, 0, 0));

    UIText::init();

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ShaderProgramManager::setActiveProgram("triangle");
        ShaderProgramManager::setMat4("projection", camera.getProjectionMat());
        ShaderProgramManager::setMat4("view", camera.getViewMat());
        
        world.draw();

        //UIText::drawText("hello", 100, 100, 48, glm::vec3(1.0f, 0.0f, 0.0f));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
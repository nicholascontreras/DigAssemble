#include "GameWindow.h"

#include <cassert>
#include <stdexcept>
#include <thread>

#include "util/debug.h"
#include "ShaderProgramManager.h"
#include "TextureManager.h"
#include "world/blocks/Block.h"
#include "world/World.h"
#include "UIText.h"
#include "util/FPSCounter.h"

Camera GameWindow::camera;
int GameWindow::width = 800, GameWindow::height = 600;

void GameWindow::run() {
    glfwInit();

    assertTypes();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    GLFWwindow* window = glfwCreateWindow(width, height, "Window Title", nullptr, nullptr);
    if(window == nullptr) {
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    Debug(glGetString(GL_VERSION));

    glEnable(GL_DEPTH_TEST);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(handleError, nullptr);

    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, windowSizeChanged);
    camera.setAspectRatio((float)width / height);

    ShaderProgramManager::compileProgram("triangle");
    ShaderProgramManager::compileProgram("text");
    TextureManager::generateTexture("container");

    Block::init();
    UIText::init();

    World world;
    world.setBlock(new Block(0, 0, 0));

    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ShaderProgramManager::setActiveProgram("triangle");
        ShaderProgramManager::setMat4("projection", camera.getProjectionMat());
        ShaderProgramManager::setMat4("view", camera.getViewMat());

        world.draw();

        UIText::drawText("FPS: " + std::to_string(FPSCounter::getFPS()), 10, 20, 20, glm::vec3(0.0f, 0.0f, 0.0f));

        glfwSwapBuffers(window);
        FPSCounter::recordFrame();

        glfwPollEvents();

        FPSCounter::delayForFPS(60);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}

int GameWindow::getWidth() {
    return width;
}

int GameWindow::getHeight() {
    return height;
}

void GameWindow::assertTypes() {
    assert(sizeof(GLenum) == sizeof(unsigned int));
    assert(sizeof(GLint) == sizeof(int));
    assert(sizeof(GLuint) == sizeof(unsigned int));
    assert(sizeof(GLsizei) == sizeof(int));
    assert(sizeof(GLchar) == sizeof(char));
    assert(sizeof(GLubyte) == sizeof(unsigned char));

    assert(GL_FALSE == false);
    assert(GL_TRUE == true);
}

void GLAPIENTRY GameWindow::handleError(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam) {
    UNREFERENCED_PARAMETER(source);
    UNREFERENCED_PARAMETER(type);
    UNREFERENCED_PARAMETER(id);
    UNREFERENCED_PARAMETER(severity);
    UNREFERENCED_PARAMETER(length);
    UNREFERENCED_PARAMETER(userParam);

    throw std::runtime_error(message);
};

void GameWindow::windowSizeChanged(GLFWwindow* window, int newWidth, int newHeight) {
    UNREFERENCED_PARAMETER(window);

    glViewport(0, 0, newWidth, newHeight);
    camera.setAspectRatio((float)newWidth / newHeight);

    width = newWidth;
    height = newHeight;
}

void GameWindow::processInput(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

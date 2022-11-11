#include "GameWindow.h"

#include <cassert>
#include <stdexcept>

#include "util/debug.h"
#include "managers/ShaderProgramManager.h"
#include "managers/TextureMapManager.h"
#include "managers/TextureManager.h"
#include "world/blocks/Block.h"
#include "world/World.h"
#include "UIText.h"
#include "util/FPSCounter.h"
#include "world/worldgen/WorldGen.h"
#include "world/WorldFile.h"
#include "world/WorldStreaming.h"
#include "util/math_utils.h"
#include "util/async/AsyncWorker.h"
#include "world/WorldFile.h"

Player GameWindow::player;
Camera GameWindow::camera;


int GameWindow::width = 800, GameWindow::height = 600;
float GameWindow::mouseX = NAN, GameWindow::mouseY = NAN;
bool GameWindow::showMouse = false;

void GameWindow::run() {
    if(!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW!");
    }

    assertTypes();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    GLFWwindow* window = glfwCreateWindow(width, height, "Window Title", nullptr, nullptr);
    if(window == nullptr) {
        throw std::runtime_error("Failed to create GLFW window!");
    }

    glfwMakeContextCurrent(window);
    if(!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD!");
    }

    Debug("OpenGL version ", glGetString(GL_VERSION));

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(handleError, nullptr);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(0, 0, width, height);

    glfwSetFramebufferSizeCallback(window, windowSizeChanged);
    camera.setAspectRatio((float)width / height);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mousePositionChanged);

    glfwSetKeyCallback(window, keyStateChanged);

    ShaderProgramManager::compileProgram("triangle");
    ShaderProgramManager::setActiveProgram("triangle");
    ShaderProgramManager::setInt("startFogDistance", (camera.getRenderDistance() - 1) * Chunk::SIZE);
    ShaderProgramManager::setInt("fullyFogDistance", camera.getRenderDistance() * Chunk::SIZE);
    ShaderProgramManager::setVec3("fogColor", glm::vec3(0.5f, 0.75f, 1.0f));
    TextureMapManager::generateTextureMap("blocks");

    UIText::init();
    AsyncWorker::start();

    WorldStreaming::RENDER_DISTANCE = camera.getRenderDistance() + 2;
    WorldStreaming::SAVE_FOLDER_PATH = "C:/Users/rf741f/Desktop/saves/world0";

    bool load = false;
    World* wp;
    if(load) {
        wp = new World(WorldFile::loadWorld("C:/Users/rf741f/Desktop/saves/world0"));
    } else {
        WorldGen::setSeed(0);
        wp = new World(WorldGen::generateNewWorld());
    }
    World world = World(std::move(*wp));
    delete wp;

    player.setY((float)world.spawnY);

    WorldStreaming::start(world, player);

    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        AsyncWorker::runCallback();

        if(width > 0) {
            glClearColor(0.5f, 0.75f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            camera.setCameraPos(player.getX(), player.getY() + 1.5f, player.getZ());
            player.setDirectionFacing(camera.getAngleLR());

            ShaderProgramManager::setActiveProgram("triangle");
            ShaderProgramManager::setMat4("projection", camera.getProjectionMat());
            ShaderProgramManager::setMat4("view", camera.getViewMat());

            world.draw();

            UIText::drawText("FPS: " + std::to_string(FPSCounter::getFPS()), 10, 20, 20, glm::vec3(0.0f, 0.0f, 0.0f));
            UIText::drawText("X: " + std::to_string(player.getX()), 10, 40, 20, glm::vec3(0.0f, 0.0f, 0.0f));
            UIText::drawText("Y: " + std::to_string(player.getY()), 10, 60, 20, glm::vec3(0.0f, 0.0f, 0.0f));
            UIText::drawText("Z: " + std::to_string(player.getZ()), 10, 80, 20, glm::vec3(0.0f, 0.0f, 0.0f));
            UIText::drawText("A: " + std::to_string(camera.getAngleLR()), 10, 100, 20, glm::vec3(0.0f, 0.0f, 0.0f));

            glfwSwapBuffers(window);
            handleContinuousKeys(window);
        }

        FPSCounter::recordFrame();
        FPSCounter::delayForFPS(60);
    }

    Async::stopAllThreads();
    AsyncWorker::finalizeAll();
    Async::joinAllThreads();

    glfwDestroyWindow(window);
    glfwTerminate();

    //WorldFile::saveWorld(world, "C:/Users/rf741f/Desktop/saves/world0");
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
}

void GameWindow::windowSizeChanged(GLFWwindow* window, int newWidth, int newHeight) {
    UNREFERENCED_PARAMETER(window);

    glViewport(0, 0, newWidth, newHeight);
    camera.setAspectRatio((float)newWidth / newHeight);

    width = newWidth;
    height = newHeight;
}

void GameWindow::mousePositionChanged(GLFWwindow* window, double posX, double posY) {
    UNREFERENCED_PARAMETER(window);

    if(showMouse) {
        // If the mouse is moved while the cursor is being shown (not controlling the camera)...
        // ...clear out previous positions and prevent any camera movement
        mouseX = NAN;
        mouseY = NAN;
        return;
    }

    bool firstTime = isnan(mouseX);

    float newMouseX = (float)posX;
    float newMouseY = (float)posY;

    if(!firstTime) {
        float mouseSensitvity = 0.1f;
        camera.rotateLR((mouseX - newMouseX) * mouseSensitvity);
        camera.rotateUD((mouseY - newMouseY) * mouseSensitvity);
    }

    mouseX = newMouseX;
    mouseY = newMouseY;
}

void GameWindow::keyStateChanged(GLFWwindow* window, int key, int scancode, int action, int mods) {
    UNREFERENCED_PARAMETER(scancode);
    UNREFERENCED_PARAMETER(mods);

    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        showMouse = !showMouse;
        glfwSetInputMode(window, GLFW_CURSOR, showMouse ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    }
}

void GameWindow::handleContinuousKeys(GLFWwindow* window) {
    if(showMouse) {
        return;
    }

    float forwardBackwardMovementSpeed = 0.2f;
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        player.moveForwardBackwards(forwardBackwardMovementSpeed);
    } else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        player.moveForwardBackwards(-forwardBackwardMovementSpeed);
    }

    float leftRightMovementSpeed = 0.1f;
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        player.strafeLeftRight(leftRightMovementSpeed);
    } else if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        player.strafeLeftRight(-leftRightMovementSpeed);
    }

    float flyUpDownSpeed = 0.2f;
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        player.flyUpDown(flyUpDownSpeed);
    } else if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        player.flyUpDown(-flyUpDownSpeed);
    }
}

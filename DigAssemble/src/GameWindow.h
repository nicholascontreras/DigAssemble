#pragma once

#include <Windows.h>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "Camera.h"
#include "world/Player.h"

class GameWindow {
public:
    static void run();

    static int getWidth();
    static int getHeight();
private:
    static Camera camera;
    static Player player;

    static int width, height;

    static float mouseX, mouseY;
    static bool showMouse;

    static void assertTypes();
    static void GLAPIENTRY handleError(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam);
    static void windowSizeChanged(GLFWwindow* window, int width, int height);
    static void mousePositionChanged(GLFWwindow* window, double posX, double posY);
    static void keyStateChanged(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void handleContinuousKeys(GLFWwindow* window);
};


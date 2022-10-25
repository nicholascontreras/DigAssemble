#pragma once

#include <Windows.h>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "Camera.h"

class GameWindow {
public:
    static void run();

    static int getWidth();
    static int getHeight();
private:
    static Camera camera;
    static int width, height;

    static void assertTypes();
    static void GLAPIENTRY handleError(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam);
    static void windowSizeChanged(GLFWwindow* window, int width, int height);
    static void processInput(GLFWwindow* window);
};


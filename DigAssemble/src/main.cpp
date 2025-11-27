// Include Header-only libraries that need defines set
#define STB_IMAGE_IMPLEMENTATION
#pragma warning (push)
#pragma warning (disable: 6262 6385 26451)
#include <stb_image/stb_image.h>
#pragma warning (pop)
#undef STB_IMAGE_IMPLEMENTATION

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#undef GLAD_GL_IMPLEMENTATION
// ---


#include <Windows.h>

#include "GameWindow.h"

int main(int argc, char** argv) {
    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    GameWindow::run();

    return 0;
}
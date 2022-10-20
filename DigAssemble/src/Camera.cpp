#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() :
    x(0), y(0), z(-5), angleLR(0), angleUD(0) {
}

glm::mat4 Camera::getViewMat() const {
    return glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
}

glm::mat4 Camera::getProjectionMat() const {
    return glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
}

#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() :
    x(0.5), y(0.5), z(0.5), angleLR(0), angleUD(0), aspectRatio(NAN) {
}

void Camera::setAspectRatio(float newAspectRatio) {
    aspectRatio = newAspectRatio;
}

void Camera::rotateLR(float amount) {
    angleLR += amount;
}

void Camera::rotateUD(float amount) {
    angleUD += amount;
}

void Camera::setCameraPos(float newX, float newY, float newZ) {
    x = newX;
    y = newY;
    z = newZ;
}

glm::mat4 Camera::getViewMat() const {
    glm::vec3 eye(x, y, z);
    
    glm::mat4 rotationMat = glm::rotate(glm::rotate(glm::mat4(1), glm::radians(angleLR), glm::vec3(0, 1, 0)), glm::radians(angleUD), glm::vec3(1, 0, 0));
    glm::vec3 lookAt = eye + glm::vec3(rotationMat * glm::vec4(0, 0, -1, 1));

    return glm::lookAt(eye, lookAt, glm::vec3(0, 1, 0));
}

glm::mat4 Camera::getProjectionMat() const {
    return glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
}

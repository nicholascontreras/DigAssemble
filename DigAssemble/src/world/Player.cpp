#include "Player.h"

#include <stdexcept>

#include <glm/gtc/matrix_transform.hpp>

Player::Player() : x(0), y(16), z(0), angleLR(NAN) {
}

void Player::moveForwardBackwards(float amount) {
    glm::vec3 movement = getDirectionFacingVector() * amount;
    
    x += movement.x;
    z += movement.z;
}

void Player::strafeLeftRight(float amount) {
    glm::vec3 movement = glm::cross(glm::vec3(0, 1, 0), getDirectionFacingVector()) * amount;

    x += movement.x;
    z += movement.z;
}

void Player::flyUpDown(float amount) {
    y += amount;
}

void Player::setDirectionFacing(float newAngleLR) {
    angleLR = newAngleLR;
}

glm::vec3 Player::getDirectionFacingVector() {
    glm::mat4 rotationMat = glm::rotate(glm::mat4(1), glm::radians(angleLR), glm::vec3(0, 1, 0));
    glm::vec3 facing = rotationMat * glm::vec4(0, 0, -1, 1);
    if(facing.y != 0) {
        throw std::runtime_error("Unexpected vertical component!");
    } 
    return facing;
}

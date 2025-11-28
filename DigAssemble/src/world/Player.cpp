#include "Player.h"

#include <stdexcept>

#include <glm/gtc/matrix_transform.hpp>

#include "Chunk.h"

const float Player::HEIGHT = 1.9f, Player::WIDTH = 0.8f;


Player::Player() : x(8.0f), y(NAN), z(8.0f), angleLR(NAN) {
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

#include "Player.h"

#include <stdexcept>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Player::Player() : x(0), y(0), z(0), angleLR(0) {
}

void Player::moveForwardBackwards(float amount) {
    glm::vec3 movement = glm::rotate(glm::mat4(1), glm::radians(angleLR), glm::vec3(0, 1, 0)) * glm::vec4(0, 0, -amount, 1);
    
    if(movement.y != 0) {
        throw std::runtime_error("Unexpected vertical movement component!");
    }
    
    x += movement.x;
    z += movement.z;
}

void Player::setDirectionFacing(float newAngleLR) {
    angleLR = newAngleLR;
}

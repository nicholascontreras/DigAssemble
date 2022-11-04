#pragma once

#include <glm/glm.hpp>

class Player {
public:
    Player();

    void moveForwardBackwards(float amount);
    void strafeLeftRight(float amount);
    void flyUpDown(float amount);

    inline void setY(float newY) { y = newY; };

    inline float getX() const { return x; };
    inline float getY() const { return y; };
    inline float getZ() const { return z; };

    void setDirectionFacing(float newAngleLR);

private:
    float x, y, z;
    float angleLR;

    glm::vec3 getDirectionFacingVector();
};


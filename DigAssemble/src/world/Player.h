#pragma once
class Player {
public:
    Player();

    void moveForwardBackwards(float amount);

    inline float getX() { return x; };
    inline float getY() { return y; };
    inline float getZ() { return z; };

    void setDirectionFacing(float newAngleLR);

private:
    float x, y, z;
    float angleLR;
};


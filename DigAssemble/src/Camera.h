#pragma once

#include <glm/glm.hpp>

class Camera {
public:
    Camera();

    void setAspectRatio(float newAspectRatio);

    void rotateLR(float amount);
    void rotateUD(float amount);

    inline float getAngleLR() { return angleLR; };

    void setCameraPos(float newX, float newY, float newZ);

    glm::mat4 getViewMat() const;
    glm::mat4 getProjectionMat() const;
private:
    float x, y, z;
    float angleLR, angleUD;
    float aspectRatio;
};


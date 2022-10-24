#pragma once

#include <glm/glm.hpp>

class Camera {
public:
    Camera(float aspectRatio);

    void setAspectRatio(float newAspectRatio);

    glm::mat4 getViewMat() const;
    glm::mat4 getProjectionMat() const;
private:
    float x, y, z;
    float angleLR, angleUD;
    float aspectRatio;
};


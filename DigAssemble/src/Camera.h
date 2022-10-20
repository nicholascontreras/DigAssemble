#pragma once

#include <glm/glm.hpp>

class Camera {
public:
    Camera();

    glm::mat4 getViewMat() const;
    glm::mat4 getProjectionMat() const;
private:
    float x, y, z;
    float angleLR, angleUD;
};


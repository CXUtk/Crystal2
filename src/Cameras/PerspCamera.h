#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class PerspCamera {
public:
    PerspCamera() = default;
    PerspCamera(glm::vec3 eyePos, glm::vec3 lookat, glm::vec3 up, float fov, float aspect, float zNear, float zFar);


    glm::mat4 GetViewTransform() const;
    glm::mat4 GetProjectionTransform() const;

    void SetEyePos(glm::vec3 pos) { eyePos = pos; }
    glm::vec3 GetEyePos() const { return eyePos; }
    glm::vec3 GetDir(glm::vec2 pos);

private:
    ~PerspCamera() = default;
    glm::vec3 eyePos, lookAt, up;
    float fov, aspect, zNear, zFar;

    glm::vec3 _up, _left, _look;
    float H, W;
};

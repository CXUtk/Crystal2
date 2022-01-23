#include "PerspCamera.h"

PerspCamera::PerspCamera(glm::vec3 eyePos, glm::vec3 lookat, glm::vec3 up, float fov, float aspect, float zNear, float zFar)
    :eyePos(eyePos), lookAt(lookat), up(up), fov(fov), aspect(aspect), zNear(zNear), zFar(zFar) {

    H = zNear * std::tan(fov / 2);
    W = H * aspect;

    _look = glm::normalize(lookAt - eyePos);
    auto p = glm::dot(up, _look) * _look;
    _up = glm::normalize(up - p);
    _right = glm::normalize(glm::cross(_look, up));
}

glm::mat4 PerspCamera::GetViewTransform() const {
    return glm::lookAt(eyePos, lookAt, up);
}

glm::mat4 PerspCamera::GetProjectionTransform() const {
    return glm::perspective(fov, aspect, zNear, zFar);
}

Ray3f PerspCamera::GenerateRay(glm::vec2 pos) {
    pos.x = pos.x * 2 - 1;
    pos.y = pos.y * 2 - 1;
    pos.x *= W, pos.y *= H;
    auto dir = glm::normalize(_right * pos.x + _up * pos.y + _look * zNear);
    return Ray3f(eyePos, dir);
}

#include "PerspCamera.h"

PerspCamera::PerspCamera(glm::vec3 eyePos, glm::vec3 lookat, glm::vec3 up, float fov, float aspect, float zNear, float zFar)
    :eyePos(eyePos), lookAt(lookat), up(up), fov(fov), aspect(aspect), zNear(zNear), zFar(zFar) {

    H = zNear * tan(fov / 2);
    W = H * aspect;

    _look = glm::normalize(lookAt - eyePos);
    auto p = glm::dot(up, _look) * _look;
    _up = glm::normalize(up - p);
    _left = glm::normalize(glm::cross(_look, up));
}

glm::mat4 PerspCamera::GetViewTransform() const {
    return glm::lookAt(eyePos, lookAt, up);
}

glm::mat4 PerspCamera::GetProjectionTransform() const {
    return glm::perspective(fov, aspect, zNear, zFar);
}

glm::vec3 PerspCamera::GetDir(float x, float y) {
    x = x * 2 - 1;
    y = y * 2 - 1;
    x *= W, y *= H;
    return glm::normalize(_left * x + _up * y + _look * zNear);
}

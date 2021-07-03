#include "PointLight.h"
#include <glm/gtx/transform.hpp>

PointLight::PointLight(glm::vec3 pos, glm::vec3 power) : _pos(pos), _power(power) {
}

PointLight::~PointLight() {
}

glm::vec3 PointLight::Sample_Li(const SurfaceInteraction& hit, const glm::vec2& sample, glm::vec3* endpoint, float* pdf) const {
    *endpoint = _pos;
    *pdf = 1.0f;
    return Flux() / (4 * glm::pi<float>());
}

glm::vec3 PointLight::Flux() const {
    return _power;
}

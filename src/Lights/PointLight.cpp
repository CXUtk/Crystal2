#include "PointLight.h"
#include <Core/SurfaceInteraction.h>
#include <Core/Utils.h>
#include <glm/gtx/transform.hpp>

crystal::PointLight::PointLight(glm::vec3 pos, glm::vec3 power) 
    : Light(crystal::LightFlags::DeltaPosition, 1), _pos(pos), _flux(power) {
}

crystal::PointLight::~PointLight() {
}

glm::vec3 crystal::PointLight::Sample_Li(const SurfaceInteraction& hit, const glm::vec2& sample, glm::vec3* endpoint, float* pdf) const {
    *endpoint = _pos;
    *pdf = 1.0f;
    return Flux() / (4 * glm::pi<float>());
}


glm::vec3 crystal::PointLight::Flux() const {
    return _flux;
}

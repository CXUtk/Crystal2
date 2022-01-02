#include "PointLight.h"
#include <Core/SurfaceInteraction.h>
#include <Core/Utils.h>
#include <glm/gtx/transform.hpp>

crystal::PointLight::PointLight(glm::vec3 pos, glm::vec3 power) 
    : Light(crystal::LightFlags::DeltaPosition, 1), _pos(pos), _flux(power) {
}

crystal::PointLight::~PointLight() {
}

Spectrum crystal::PointLight::Sample_Li(const SurfaceInfo& surface, const glm::vec2& sample,
    Point3f* endpoint, float* pdf) const {
    *endpoint = _pos;
    *pdf = 1.0f;
    auto I = Flux() / (4 * glm::pi<float>());
    return I / sqr(*endpoint - surface.GetPosition());
}

float crystal::PointLight::Pdf_Li(const SurfaceInfo& surface_w, const Vector3f& wi) const
{
    return 0.0f;
}


Spectrum crystal::PointLight::Flux() const {
    return _flux;
}

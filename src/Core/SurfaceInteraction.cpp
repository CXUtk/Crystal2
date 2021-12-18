#include "SurfaceInteraction.h"
#include <Shapes/Shape.h>
#include <Core/Prototype.h>
#include <Core/Entities.h>

static constexpr float EPS = 1e-4f;

SurfaceInteraction::SurfaceInteraction() {

}

Ray SurfaceInteraction::SpawnRay(glm::vec3 dir, bool pass) const {
    bool back = glm::dot(dir, _normal) < 0;
    return Ray(_hitPos + ((back ^ pass) ? -_normal : _normal) * EPS, dir);
}

void SurfaceInteraction::SetHitInfo(float t, const glm::vec3& hitPos, const glm::vec3& hitDir, const glm::vec3& normal,
        glm::vec2 uv, bool frontFace, const glm::vec3& dpdu, const glm::vec3& dpdv)
{
    _hitPos = hitPos;
    _distance = t;
    _hitDir = hitDir;
    _uv = uv;
    _normal = normal;
    _frontFace = frontFace;
    _dpdu = dpdu;
    _dpdv = dpdv;
}

glm::mat3 SurfaceInteraction::GetTNB() const
{
    return glm::mat3(_dpdu, _normal, _dpdv);
}

glm::mat3 SurfaceInteraction::GetInverseTNB() const
{
    return glm::transpose(GetTNB());
}

SurfaceInfo SurfaceInteraction::GetSurfaceInfo() const
{
    return SurfaceInfo(_hitPos, _frontFace ? _normal : -_normal);
}

Spectrum SurfaceInteraction::Le(const Vector3f& w) const
{
    auto areaLight = _hitEntity->GetAreaLight();
    return areaLight ? areaLight->EvalEmission(GetSurfaceInfo(), w) : Spectrum(0.f);
}

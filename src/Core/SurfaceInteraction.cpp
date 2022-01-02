#include "SurfaceInteraction.h"
#include <Shapes/Shape.h>
#include <Core/Prototype.h>
#include <Core/Entities.h>

static constexpr float EPS = 1e-4f;

SurfaceInteraction::SurfaceInteraction() {

}

Ray SurfaceInteraction::SpawnRay(glm::vec3 dir) const {
    bool back = glm::dot(dir, _normal) < 0;
    return Ray(_hitPos + ((back) ? -_normal : _normal) * EPS, dir);
}

Ray SurfaceInteraction::SpawnRayTo(const Point3f& pos) const
{
    bool back = glm::dot(pos - _hitPos, _normal) < 0;
    Point3f origin = _hitPos + ((back) ? -_normal : _normal) * EPS;
    return Ray(origin, pos - _hitPos);
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

SurfaceInfo SurfaceInteraction::GetSurfaceInfo(bool actualNormal) const
{
    return SurfaceInfo(_hitPos, actualNormal ? _normal 
        : (_frontFace ? _normal : -_normal));
}

Spectrum SurfaceInteraction::Le(const Vector3f& w) const
{
    auto areaLight = _hitEntity->GetAreaLight();
    return areaLight ? areaLight->EvalEmitRadiance(GetSurfaceInfo(true), w) : Spectrum(0.f);
}

Ray SurfaceInfo::SpawnRay(glm::vec3 dir) const
{
    return Ray(_pos + _normal * EPS, dir);
}

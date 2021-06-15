#include "SurfaceInteraction.h"

static constexpr float EPS = 1e-4f;

SurfaceInteraction::SurfaceInteraction() : _hitObject(nullptr), _hitShape(nullptr),
    _distance(std::numeric_limits<float>::infinity()),
    _hitPos(0), _uv(0), _normal(0), _dpdu(0), _dpdv(0),
    _frontFace(false) {

}

SurfaceInteraction::~SurfaceInteraction() {
}

Ray SurfaceInteraction::SpawnRay(glm::vec3 dir, bool pass) const {
    bool back = glm::dot(dir, _normal) < 0;
    return Ray(_hitPos + ((back ^ pass) ? -_normal : _normal) * EPS, dir);
}

void SurfaceInteraction::SetHitInfo(float t, glm::vec3 hitPos, glm::vec3 normal, glm::vec2 uv, bool frontFace,
    const Shape* shape, glm::vec3 dpdu, glm::vec3 dpdv) {
    _hitPos = hitPos;
    _distance = t;
    _hitShape = shape;
    _uv = uv;
    _normal = normal;
    _frontFace = frontFace;
    _dpdu = dpdu;
    _dpdv = dpdv;
}

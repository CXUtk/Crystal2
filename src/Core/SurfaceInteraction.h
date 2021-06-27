#pragma once
#include <Crystal2.h>
#include <Core/Geometry.h>
#include <glm/glm.hpp>
#include <memory>

class SurfaceInteraction {
public:
    SurfaceInteraction();

    Ray SpawnRay(glm::vec3 dir, bool pass = false) const;
    void SetHitInfo(float t, glm::vec3 hitPos, glm::vec3 normal, glm::vec2 uv,
        bool frontFace, const Shape* shape, glm::vec3 dpdu, glm::vec3 dpdv);

    bool IsFrontFace() const { return _frontFace; }
    float GetDistance() const { return _distance; }
    const Shape* GetHitShape() const { return _hitShape; }
    std::shared_ptr<Prototype> GetHitPrototype() const;
    glm::vec3 GetHitPos() const { return _hitPos; }
    glm::vec3 GetNormal() const { return _normal; }
    glm::vec2 GetUV() const { return _uv; }
    glm::vec3 GetDpDu() const { return _dpdu; }
    glm::vec3 GetDpDv() const { return _dpdv; }


private:
    const Shape* _hitShape;

    float _distance;
    bool _frontFace;
    glm::vec3 _hitPos, _normal;
    glm::vec2 _uv;
    glm::vec3 _dpdu, _dpdv;
};
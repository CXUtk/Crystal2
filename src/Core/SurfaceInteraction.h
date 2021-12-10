#pragma once
#include <Crystal2.h>
#include <Core/Geometry.h>
#include <memory>

class SurfaceInteraction {
public:
    SurfaceInteraction();

    Ray SpawnRay(glm::vec3 dir, bool pass = false) const;
    void SetHitInfo(float t, const glm::vec3& hitPos, const glm::vec3& hitDir, const glm::vec3& normal, 
        glm::vec2 uv, bool frontFace, const Shape* shape, const glm::vec3& dpdu, const glm::vec3& dpdv);

    bool IsFrontFace() const { return _frontFace; }
    float GetDistance() const { return _distance; }
    const Shape* GetHitShape() const { return _hitShape; }
    const Prototype* GetHitPrototype() const;
    glm::vec3 GetHitPos() const { return _hitPos; }
    glm::vec3 GetNormal() const { return _normal; }
    glm::vec2 GetUV() const { return _uv; }
    glm::vec3 GetDpDu() const { return _dpdu; }
    glm::vec3 GetDpDv() const { return _dpdv; }
    glm::vec3 GetHitDir() const { return _dir; }

    void SetBSDF(BSDF* bsdf) { _bsdf = bsdf; }
    BSDF* GetBSDF() const { return _bsdf; }

private:
    const Shape* _hitShape = nullptr;
    BSDF* _bsdf = nullptr;

    float _distance = std::numeric_limits<float>::infinity();
    bool _frontFace = false;
    glm::vec3 _hitPos{}, _normal{}, _dir{};
    glm::vec2 _uv{};
    glm::vec3 _dpdu{}, _dpdv{};
};
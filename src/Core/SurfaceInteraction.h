#pragma once
#include <Crystal2.h>
#include <Core/Geometry.h>
#include <memory>

class SurfaceInfo
{
public:
    SurfaceInfo(const Point3f& position, const Normal3f& normal) :_pos(position), _normal(normal) {}
    Ray SpawnRay(glm::vec3 dir) const;
    Point3f GetPosition() const { return _pos; }
    Normal3f GetNormal() const { return _normal; }

private:
    Point3f _pos;
    Normal3f _normal;
};

class SurfaceInteraction {
public:
    SurfaceInteraction();

    Ray SpawnRay(glm::vec3 dir) const;
    Ray SpawnRayTo(const Point3f& pos) const;

    void SetHitInfo(float t, const glm::vec3& hitPos, const glm::vec3& hitDir, const glm::vec3& normal, 
        glm::vec2 uv, bool frontFace, const glm::vec3& dpdu, const glm::vec3& dpdv);

    bool IsFrontFace() const { return _frontFace; }
    float GetDistance() const { return _distance; }

    void SetHitEntity(const crystal::Entity* entity) { _hitEntity = entity; }
    const crystal::Entity* GetHitEntity() const { return _hitEntity; }

    glm::vec3 GetHitPos() const { return _hitPos; }
    glm::vec3 GetNormal() const { return _normal; }
    glm::vec2 GetUV() const { return _uv; }
    glm::vec3 GetDpDu() const { return _dpdu; }
    glm::vec3 GetDpDv() const { return _dpdv; }
    glm::vec3 GetHitDir() const { return _hitDir; }
    glm::mat3 GetTNB() const;
    glm::mat3 GetInverseTNB() const;

    SurfaceInfo GetSurfaceInfo(bool actualNormal) const;

    void SetBSDF(BSDF* bsdf) { _bsdf = bsdf; }
    BSDF* GetBSDF() const { return _bsdf; }

    /**
     * @brief Get the emitted radiance at a surface point intersected by a ray
     * @param w Direction of the ray in world coord
     * @return 
    */
    Spectrum Le(const Vector3f& w) const;

private:
    const crystal::Entity* _hitEntity = nullptr;
    BSDF* _bsdf = nullptr;

    float _distance = std::numeric_limits<float>::infinity();
    bool _frontFace = false;
    Point3f _hitPos{};
    Vector3f _hitDir{}, _dpdu{}, _dpdv{};
    Normal3f _normal{};
    Vector2f _uv{};
};
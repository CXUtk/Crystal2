#pragma once
#include <Core/Geometry.h>
#include <Core/SurfaceInteraction.h>
#include "Shape.h"
#include <memory>

class Sphere : public Shape
{
public:
    static std::shared_ptr<Shape> CreateSphere(JsonNode_CPTR pShapeNode);
    Sphere(glm::vec3 pos, float radius, glm::vec3 rotation);
    ~Sphere() override;
    Bound3f GetBoundingBox() const override;
    bool Intersect(const Ray3f& ray, SurfaceInteraction* isec) const override;
    bool IntersectTest(const Ray3f& ray, const crystal::IIntersectable* ignoreShape, float tMin = 0,
        float tMax = std::numeric_limits<float>::infinity()) const override;
    float SurfaceArea() const override;
    
    SurfaceInfo SampleSurface(const Vector2f& sample) const override;
    // 采样目标点为半球的立体角
    SurfaceInfo SampleSurfaceLight(const Vector2f& sample,
        const SurfaceInteraction& ref) const override;

private:
    glm::vec3 _pos, _rot;
    float _radius;
    glm::mat3 _world2Local;
    glm::mat3 _local2World;
};

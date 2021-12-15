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
    BoundingBox GetBoundingBox() const override;
    bool Intersect(const Ray& ray, SurfaceInteraction* isec) const override;
    bool IntersectTest(const Ray& ray, float tMin = 0,
        float tMax = std::numeric_limits<float>::infinity()) const override;
    float SurfaceArea() const override;
    SurfaceInfo SampleSurface(const Vector2f& sample, float& pdf) const override;

private:
    glm::vec3 _pos, _rot;
    float _radius;
    glm::mat3 _world2Local;
    glm::mat3 _local2World;
};

#pragma once
#include <Crystal2.h>
#include <Shapes/Shape.h>
struct VertexData;

class Triangle : public Shape {
public:
    Triangle(const VertexData* a, const VertexData* b, const VertexData* c);
    ~Triangle() override;

    BoundingBox GetBoundingBox() const override;
    bool Intersect(const Ray& ray, SurfaceInteraction* isec) const override;
    bool IntersectTest(const Ray& ray, float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const override;
    float SurfaceArea() const override;
    SurfaceInfo SampleSurface(const Vector2f& sample, float& pdf) const override;

private:
    const VertexData* _vertices[3];
    glm::vec3 _dpdu, _dpdv;
    BoundingBox _bbox;

    void calculateDerivative();
    Vector3f sampleTriangle(glm::vec2 sample, float& pdf) const;
};

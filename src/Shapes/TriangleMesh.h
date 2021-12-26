#pragma once
#include <Crystal2.h>
#include <vector>
#include <memory>
#include <Core/Utils.h>
#include <Shapes/Triangle.h>

class TriangleMesh : public Shape {
public:
    static std::shared_ptr<TriangleMesh> CreateTriangleMesh(JsonNode_CPTR pShapeNode);
    TriangleMesh(const std::vector<VertexData>& vertices,
        const std::vector<glm::ivec3> faceIndices, const glm::mat4& transform);
    ~TriangleMesh();

    BoundingBox GetBoundingBox() const override { return _boundingBox; }
    bool Intersect(const Ray& ray, SurfaceInteraction* isec) const override;
    bool IntersectTest(const Ray& ray, float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const override;
    float SurfaceArea() const override { return _surfaceArea; }
    SurfaceInfo SampleSurface(const Vector2f& sample, float* pdf) const override;

private:
    int _numVertices;
    std::unique_ptr<VertexData[]> _vertices;
    std::vector<std::shared_ptr<Triangle>> _triangles;
    std::unique_ptr<Accelerator> _accelStructure;
    glm::mat4 _transform;

    BoundingBox _boundingBox;
    float _surfaceArea;
};

#pragma once
#include <Crystal2.h>
#include <vector>
#include <memory>
#include <Core/Utils.h>
#include <Shapes/Triangle.h>

class TriangleMesh {
public:
    static std::shared_ptr<TriangleMesh> CreateTriangleMesh(JsonNode_CPTR pShapeNode);
    TriangleMesh(const std::vector<VertexData>& vertices,
        const std::vector<glm::ivec3> faceIndices, const glm::mat4& transform);
    ~TriangleMesh();

    std::vector<std::shared_ptr<Triangle>> GetTriangles() const;

    //BoundingBox GetBoundingBox() const override { return _boundingBox; }
    //bool Intersect(const Ray& ray, SurfaceInteraction* isec) const override;
    //bool IntersectTest(const Ray& ray, const crystal::IIntersectable* ignoreShape, 
    //    float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const override;
    //float SurfaceArea() const override { return _surfaceArea; }

    //SurfaceInfo SampleSurface(const Vector2f& sample) const override;
    //// 采样目标点为半球的立体角
    //SurfaceInfo SampleSurfaceLight(const Vector2f& sample,
    //    const SurfaceInteraction& ref) const override
    //{
    //    return SampleSurface(sample);
    //}

private:
    int _numVertices;
    std::unique_ptr<VertexData[]> _vertices;
    std::vector<std::shared_ptr<Triangle>> _triangles;
    glm::mat4 _transform;
};

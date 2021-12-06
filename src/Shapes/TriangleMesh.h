#pragma once
#include <Crystal2.h>
#include <vector>
#include <memory>
#include <Core/Utils.h>

class TriangleMesh {
public:
    static std::shared_ptr<TriangleMesh> CreateTriangleMesh(Prototype* prototype,
       JsonNode_CPTR pShapeNode);
    TriangleMesh(const Prototype* prototype, const std::vector<VertexData>& vertices,
        const std::vector<glm::ivec3> faceIndices, const glm::mat4& transform);
    ~TriangleMesh();

    std::vector<std::shared_ptr<Shape>> GetTriangles() const;

private:
    int _numVertices;
    std::unique_ptr<VertexData[]> _vertices;
    std::vector<glm::ivec3> _faces;
    const Prototype* _prototype;
    glm::mat4 _transform;
};

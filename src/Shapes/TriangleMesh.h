#pragma once
#include <Crystal2.h>
#include <vector>
#include <memory>
#include <Core/Utils.h>

class TriangleMesh {
public:
    TriangleMesh(const std::vector<VertexData>& vertices, const std::vector<int> faceIndices, const glm::mat4& transform);
    ~TriangleMesh();

    std::vector<std::shared_ptr<Shape>> GetTriangles() const;

private:
    int _numVertices;
    std::unique_ptr<VertexData[]> _vertices;
    std::vector<int> _indices;
    glm::mat4 _transform;
};

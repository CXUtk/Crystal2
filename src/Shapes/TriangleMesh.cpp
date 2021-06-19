#include "TriangleMesh.h"
#include "Triangle.h"
TriangleMesh::TriangleMesh(const std::vector<VertexData>& vertices,
    const std::vector<int> faceIndices,
    const glm::mat4& transform) : _numVertices(vertices.size()), _indices(faceIndices), _transform(transform) {
    _vertices = std::unique_ptr<VertexData[]>(new VertexData[_numVertices]);
    memcpy(_vertices.get(), vertices.data(), sizeof(VertexData) * _numVertices);
    glm::mat4 normalTrans = glm::transpose(glm::inverse(transform));
    for (int i = 0; i < _numVertices; i++) {
        auto& v = _vertices[i];
        v.Position = glm::vec3(_transform * glm::vec4(v.Position, 1.0f));
        v.Normal = glm::vec3(normalTrans * glm::vec4(v.Normal, 0.0f));
    }
}

TriangleMesh::~TriangleMesh() {
}

std::vector<std::shared_ptr<Shape>> TriangleMesh::GetTriangles() const {
    std::vector<std::shared_ptr<Shape>> res;
    int numIndices = _indices.size();
    for (int i = 0; i < numIndices; i += 3) {
        res.push_back(std::make_shared<Triangle>(_vertices[_indices[i]],
            _vertices[_indices[i + 1]],
            _vertices[_indices[i + 2]]));
    }
    return res;
}

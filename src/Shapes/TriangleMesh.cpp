#include "TriangleMesh.h"
#include "Triangle.h"
#include <SJson/SJson.h>
#include <glm/gtx/transform.hpp>
#include <Loaders/JsonLoader.h>
#include <Loaders/ObjLoader.h>

std::shared_ptr<TriangleMesh> TriangleMesh::CreateTriangleMesh(const std::shared_ptr<Prototype>& prototype, const std::shared_ptr<SJson::SJsonNode>& shapeNode) {
    auto filePath = shapeNode->GetMember("ObjFile")->GetString();
    ObjLoader loader;
    loader.load(filePath);

    auto pos = loader::parse_vec3(shapeNode->GetMember("Translation"));
    auto scale = loader::parse_vec3(shapeNode->GetMember("Scale"));
    auto rotation = loader::parse_vec3(shapeNode->GetMember("Rotation"));

    auto matrix = glm::identity<glm::mat4>();
    matrix = glm::translate(matrix, pos);
    matrix = glm::rotate(matrix, rotation.x, glm::vec3(1, 0, 0));
    matrix = glm::rotate(matrix, rotation.y, glm::vec3(0, 1, 0));
    matrix = glm::rotate(matrix, rotation.z, glm::vec3(0, 0, 1));
    matrix = glm::scale(matrix, scale);

    return loader.GetMesh(prototype, matrix);
}

TriangleMesh::TriangleMesh(const std::shared_ptr<Prototype>& prototype,
    const std::vector<VertexData>& vertices,
    const std::vector<glm::ivec3> faceIndices,
    const glm::mat4& transform) : _prototype(prototype), _numVertices(vertices.size()), _faces(faceIndices), _transform(transform) {
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
    int numFaces = _faces.size();
    for (int i = 0; i < numFaces; i++) {
        res.push_back(std::make_shared<Triangle>(_prototype, &_vertices[_faces[i][0]],
            &_vertices[_faces[i][1]],
            &_vertices[_faces[i][2]]));
    }
    return res;
}

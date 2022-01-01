#include "TriangleMesh.h"
#include "Triangle.h"
#include <SJson/SJson.h>
#include <glm/gtx/transform.hpp>
#include <Loaders/JsonLoader.h>
#include <Loaders/ObjLoader.h>
#include <Accelerators/Accelerator.h>

std::shared_ptr<TriangleMesh> TriangleMesh::CreateTriangleMesh(JsonNode_CPTR pShapeNode)
{
	auto filePath = pShapeNode->GetMember("ObjFile")->GetString();
	ObjLoader loader;
	loader.load(filePath);

	auto pos = loader::parse_vec3(pShapeNode->GetMember("Translation"));
	auto scale = loader::parse_vec3(pShapeNode->GetMember("Scale"));
	auto rotation = glm::radians(loader::parse_vec3(pShapeNode->GetMember("Rotation")));

	auto matrix = glm::identity<glm::mat4>();
	matrix = glm::translate(matrix, pos);
	matrix = glm::rotate(matrix, rotation.x, glm::vec3(1, 0, 0));
	matrix = glm::rotate(matrix, rotation.y, glm::vec3(0, 1, 0));
	matrix = glm::rotate(matrix, rotation.z, glm::vec3(0, 0, 1));
	matrix = glm::scale(matrix, scale);

	return loader.GetMesh(matrix);
}

TriangleMesh::TriangleMesh(const std::vector<VertexData>& vertices, const std::vector<glm::ivec3> faceIndices,
	const glm::mat4& transform) : _numVertices(vertices.size()), _transform(transform)
{
	_vertices = std::unique_ptr<VertexData[]>(new VertexData[_numVertices]);
	memcpy(_vertices.get(), vertices.data(), sizeof(VertexData) * _numVertices);
	glm::mat4 normalTrans = glm::transpose(glm::inverse(transform));

	for (int i = 0; i < _numVertices; i++)
	{
		auto& v = _vertices[i];
		v.Position = glm::vec3(_transform * glm::vec4(v.Position, 1.0f));
		v.Normal = glm::normalize(glm::vec3(normalTrans * glm::vec4(v.Normal, 0.0f)));
	}
	int numFaces = faceIndices.size();
	for (int i = 0; i < numFaces; i++)
	{
		auto& curFace = faceIndices[i];
		auto triangle = std::make_shared<Triangle>(&_vertices[curFace[0]],
			&_vertices[curFace[1]],
			&_vertices[curFace[2]]);
		_triangles.push_back(triangle);
	}
}

TriangleMesh::~TriangleMesh()
{}

std::vector<std::shared_ptr<Triangle>> TriangleMesh::GetTriangles() const
{
	return _triangles;
}

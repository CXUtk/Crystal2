#include "TriangleMesh.h"
#include "Triangle.h"
#include <SJson/SJson.h>
#include <glm/gtx/transform.hpp>
#include <Loaders/JsonLoader.h>
#include <Loaders/ObjLoader.h>

std::shared_ptr<TriangleMesh> TriangleMesh::CreateTriangleMesh(JsonNode_CPTR pShapeNode) {
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

TriangleMesh::TriangleMesh(const std::vector<VertexData>& vertices, const std::vector<glm::ivec3> faceIndices, const glm::mat4& transform)
    : _numVertices(vertices.size()), _transform(transform)
{
    _vertices = std::unique_ptr<VertexData[]>(new VertexData[_numVertices]);
    memcpy(_vertices.get(), vertices.data(), sizeof(VertexData) * _numVertices);
    glm::mat4 normalTrans = glm::transpose(glm::inverse(transform));

    glm::vec3 bbMinnPos = glm::vec3(std::numeric_limits<float>::infinity());
    glm::vec3 bbMaxxPos = glm::vec3(-std::numeric_limits<float>::infinity());
    for (int i = 0; i < _numVertices; i++)
    {
        auto& v = _vertices[i];
        v.Position = glm::vec3(_transform * glm::vec4(v.Position, 1.0f));
        v.Normal = glm::normalize(glm::vec3(normalTrans * glm::vec4(v.Normal, 0.0f)));
        bbMinnPos = glm::min(bbMinnPos, v.Position);
        bbMaxxPos = glm::max(bbMaxxPos, v.Position);
    }
    _boundingBox = BoundingBox(bbMinnPos, bbMaxxPos);

    int numFaces = faceIndices.size();
    _surfaceArea = 0.f;
    for (int i = 0; i < numFaces; i++)
    {
        auto& curFace = faceIndices[i];
        auto triangle = std::make_shared<Triangle>(&_vertices[curFace[0]],
            &_vertices[curFace[1]],
            &_vertices[curFace[2]]);
        _triangles.push_back(triangle);
        _surfaceArea += triangle->SurfaceArea();
    }
}

TriangleMesh::~TriangleMesh() {
}

bool TriangleMesh::Intersect(const Ray& ray, SurfaceInteraction* isec) const
{
    bool hit = false;
    for (auto& triangle : _triangles)
    {
        SurfaceInteraction tmp;
        if (triangle->Intersect(ray, &tmp))
        {

            auto dist = tmp.GetDistance();
            if (dist < isec->GetDistance())
            {
                *isec = tmp;
            }
            if (std::isinf(isec->GetDistance()))
            {
                printf("Time too large\n");
                printf("%.lf\n", tmp.GetDistance());
                throw;
            }
            hit = true;
        }
    }


    return hit;
}

bool TriangleMesh::IntersectTest(const Ray& ray, float tMin, float tMax) const
{
    for (auto& triangle : _triangles)
    {
        if (triangle->IntersectTest(ray, tMin, tMax))
        {
            return true;
        }
    }
    return false;
}

SurfaceInfo TriangleMesh::SampleSurface(const Vector2f& sample, float* pdf) const
{
    int size = _triangles.size();
    *pdf = 1.f / size;

    int v = (int)(std::min(0.9999999404f, sample.x) * size);
    float pdf2;
    auto surface = _triangles[v]->SampleSurface(sample, &pdf2);
    *pdf *= pdf2;
    return surface;
}

#include "Triangle.h"
#include <glm/gtx/transform.hpp>
#include <Core/Scene.h>
#include <Core/Utils.h>

constexpr bool FLAT_SHADING = false;


glm::vec3 bary_interp(glm::vec3 bary, glm::vec3 A, glm::vec3 B, glm::vec3 C) {
    return bary.x * A + bary.y * B + bary.z * C;
}
glm::vec2 bary_interp(glm::vec3 bary, glm::vec2 A, glm::vec2 B, glm::vec2 C) {
    return bary.x * A + bary.y * B + bary.z * C;
}

Triangle::Triangle(const VertexData* a, const VertexData* b, const VertexData* c) {
    _vertices[0] = a, _vertices[1] = b, _vertices[2] = c;
    calculateDerivative();
}

Triangle::~Triangle() {
}

BoundingBox Triangle::GetBoundingBox() const {
    return _bbox;
}

bool Triangle::Intersect(const Ray& ray, SurfaceInteraction* isec) const {
    glm::mat3 A(_vertices[1]->Position - _vertices[0]->Position, _vertices[2]->Position - _vertices[0]->Position, -ray.dir);
    glm::vec3 P = ray.start - _vertices[0]->Position;
    glm::vec3 res;
    // 通过求解 Ax = b 算出光线打在三角形上的重心坐标
    res = glm::inverse(A) * P;
    if (glm::isnan(res) != glm::bvec3(false)) return false;
    if (res.x < 0.f || res.x > 1.f || res.y < 0.f || res.y > 1.f || res.x + res.y > 1.0f + EPS || res.z < 0.f) return false;
    glm::vec3 bary_coord = glm::vec3(1 - res.x - res.y, res.x, res.y);
    glm::vec3 N;
    glm::vec2 UV = bary_interp(bary_coord, _vertices[0]->TexCoords, _vertices[1]->TexCoords, _vertices[2]->TexCoords);
    if (FLAT_SHADING) {
        N = glm::normalize(glm::cross(_vertices[1]->Position - _vertices[0]->Position, _vertices[2]->Position - _vertices[0]->Position));
    }
    else {
        N = glm::normalize(bary_interp(bary_coord, _vertices[0]->Normal, _vertices[1]->Normal, _vertices[2]->Normal));
    }
    auto front_face = glm::dot(ray.dir, N) < 0;
    N = front_face ? N : -N;

    auto dpdu = _dpdu;
    // 如果没有切线向量（通常是因为没有纹理坐标绑定），那么就手动计算一个
    if (dpdu == glm::vec3(0)) {
        for (int i = 0; i < 3; i++) {
            glm::vec3 v(0);
            v[i] = 1;
            auto tmp = glm::cross(v, N);
            if (tmp != glm::vec3(0)) {
                dpdu = glm::normalize(tmp);
                break;
            }
        }
    }

    isec->SetHitInfo(res.z, ray.start + ray.dir * res.z, ray.dir, N, UV, front_face, dpdu, glm::cross(N, dpdu));
    if (std::isinf(res.z) || std::isnan(res.z))
    {
        printf("Invalid distance on triangle intersection: %lf\n", res.z);
        throw;
    }
    return true;
}

bool Triangle::IntersectTest(const Ray& ray, const crystal::IIntersectable* ignoreShape, 
    float tMin, float tMax) const {
    if (tMin > tMax) return false;
    glm::mat3 A(_vertices[1]->Position - _vertices[0]->Position, _vertices[2]->Position - _vertices[0]->Position, -ray.dir);
    glm::vec3 P = ray.start - _vertices[0]->Position;
    auto res = glm::inverse(A) * P;
    if (glm::isnan(res) != glm::bvec3(false)) return false;
    if (res.x < 0 || res.x > 1 || res.y < 0 || res.y > 1 || res.x + res.y > 1 || res.z < 0) return false;
    return res.z >= tMin && res.z <= tMax;
}

float Triangle::SurfaceArea() const
{
    return glm::length(glm::cross(_vertices[1]->Position - _vertices[0]->Position, _vertices[2]->Position - _vertices[0]->Position)) / 2.f;
}

SurfaceInfo Triangle::SampleSurface(const Vector2f& sample) const
{
    Vector3f baryCoord = sampleTriangle(sample);
    Point3f pos = baryCoord.x * _vertices[0]->Position + 
        baryCoord.y * _vertices[1]->Position + baryCoord.z * _vertices[2]->Position;
    Normal3f N;
    if (FLAT_SHADING)
    {
        N = glm::normalize(glm::cross(_vertices[1]->Position - _vertices[0]->Position, _vertices[2]->Position - _vertices[0]->Position));
    }
    else
    {
        N = glm::normalize(bary_interp(baryCoord, _vertices[0]->Normal, _vertices[1]->Normal, _vertices[2]->Normal));
    }
    return SurfaceInfo(pos, N);
}

Vector3f Triangle::sampleTriangle(glm::vec2 sample) const
{
    auto u = 1 - std::sqrt(sample.x);
    auto v = sample.y * std::sqrt(sample.x);
    return Vector3f(1.f - u - v, u, v);
}


void Triangle::calculateDerivative() {
    // 通过纹理坐标算出UV方向的切线向量
    glm::mat2 A(
        glm::vec2(_vertices[0]->TexCoords.x - _vertices[2]->TexCoords.x, _vertices[0]->TexCoords.y - _vertices[2]->TexCoords.y),
        glm::vec2(_vertices[1]->TexCoords.x - _vertices[2]->TexCoords.x, _vertices[1]->TexCoords.y - _vertices[2]->TexCoords.y)
    );
    A = glm::inverse(A);

    if (glm::isinf(A[0]) != glm::bvec2(false) || glm::isnan(A[0]) != glm::bvec2(false) || glm::isnan(A[1]) != glm::bvec2(false)) {
        _dpdu = _dpdv = glm::vec3(0);
    }
    else {
        auto a = _vertices[0]->Position - _vertices[2]->Position;
        auto b = _vertices[1]->Position - _vertices[2]->Position;
        auto res = A * glm::mat3x2(glm::vec2(a.x, b.x), glm::vec2(a.y, b.y), glm::vec2(a.z, b.z));
        _dpdu = glm::normalize(glm::vec3(res[0][0], res[1][0], res[2][0]));
        _dpdv = glm::normalize(glm::vec3(res[0][1], res[1][1], res[2][1]));
    }

    glm::vec3 minn = _vertices[0]->Position, maxx = _vertices[0]->Position;
    for (int i = 1; i < 3; i++) {
        minn.x = std::min(minn.x, _vertices[i]->Position.x);
        minn.y = std::min(minn.y, _vertices[i]->Position.y);
        minn.z = std::min(minn.z, _vertices[i]->Position.z);

        maxx.x = std::max(maxx.x, _vertices[i]->Position.x);
        maxx.y = std::max(maxx.y, _vertices[i]->Position.y);
        maxx.z = std::max(maxx.z, _vertices[i]->Position.z);
    }
    _bbox = BoundingBox(minn, maxx);
}



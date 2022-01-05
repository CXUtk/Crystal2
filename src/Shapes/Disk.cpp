#include "Disk.h"
#include <glm/gtx/transform.hpp>
#include <SJson/SJson.h>
#include <Loaders/JsonLoader.h>
#include <Core/Utils.h>

std::shared_ptr<Shape> crystal::Disk::CreateDisk(JsonNode_CPTR pShapeNode)
{
    auto pos = loader::parse_vec3(pShapeNode->GetMember("Position"));
    auto r = pShapeNode->GetMember("Radius")->GetFloat();
    Vector3f rotation = glm::vec3(0);
    if (pShapeNode->HasMember("Rotation"))
    {
        rotation = loader::parse_vec3(pShapeNode->GetMember("Rotation"));
    }
    return std::make_shared<Disk>(pos, r, rotation);
}

crystal::Disk::Disk(const Point3f& pos, float radius, const Vector3f& rotation)
	: _pos(pos), _radius(radius), _rot(rotation)
{
    glm::mat4 rotMatrix = glm::identity<glm::mat4>();
    rotMatrix = glm::rotate(rotMatrix, rotation.x, glm::vec3(1, 0, 0));
    rotMatrix = glm::rotate(rotMatrix, rotation.y, glm::vec3(0, 1, 0));
    rotMatrix = glm::rotate(rotMatrix, rotation.z, glm::vec3(0, 0, 1));

    _local2World = glm::mat3(rotMatrix);
    _world2Local = glm::transpose(_local2World);
}

crystal::Disk::~Disk()
{}

BoundingBox crystal::Disk::GetBoundingBox() const
{
    Normal3f worldNormal = _local2World * Vector3f(0, 1, 0);
    float sinTheta = std::sqrt(1.f - worldNormal.y * worldNormal.y);
    float absCosTheta = std::abs(worldNormal.y);
    NAN_DETECT_V(sinTheta, "Disk::SinTheta");

    Vector3f offset = Vector3f(_radius, _radius * sinTheta, _radius);
    return BoundingBox(_pos - offset, _pos + offset);
}

bool crystal::Disk::Intersect(const Ray& ray, SurfaceInteraction* isec) const
{
    glm::vec3 P = _world2Local * (ray.start - _pos);
    glm::vec3 d = _world2Local * ray.dir;
    if (d.y == 0.f) return false;
    float t = -P.y / d.y;
    if (t < 0) return false;
    Point3f hit = P + d * t;
    if (glm::dot(hit, hit) > _radius * _radius) return false;
    
    Normal3f N = _local2World * Vector3f(0, 1, 0);
    auto front_face = d.y <= 0;
    N = front_face ? N : -N;
    Point3f hit_world = _local2World * hit + _pos;
    auto dpdu = glm::normalize(_pos - hit_world);
    auto dpdv = glm::cross(N, dpdu);

    auto u = glm::length(hit) / _radius;
    auto v = (std::atan2(-hit.z, hit.x) + glm::pi<float>()) / glm::two_pi<float>();
    isec->SetHitInfo(t, hit_world, ray.dir, N, glm::vec2(u, v), front_face, dpdu, dpdv);
    return true;
}

bool crystal::Disk::IntersectTest(const Ray& ray, const crystal::IIntersectable* ignoreShape, float tMin, float tMax) const
{
    glm::vec3 P = _world2Local * (ray.start - _pos);
    glm::vec3 d = _world2Local * ray.dir;
    if (d.y == 0.f) return false;
    float t = -P.y / d.y;
    if (t < tMin || t > tMax) return false;
    Point3f hit = P + d * t;
    if (glm::dot(hit, hit) > _radius * _radius) return false;
    return true;
}

float crystal::Disk::SurfaceArea() const
{
    return glm::pi<float>() * _radius * _radius;
}

SurfaceInfo crystal::Disk::SampleSurface(const Vector2f& sample) const
{
    auto r = std::sqrt(sample.x);
    auto phi = sample.y * glm::two_pi<float>();

    Vector3f d_world = _local2World * Vector3f(std::cos(phi) * r * _radius, 0.f, -std::sin(phi) * r * _radius);
    Normal3f N_world = _local2World * Vector3f(0, 1, 0);
    return SurfaceInfo(_pos + d_world, N_world);
}

SurfaceInfo crystal::Disk::SampleSurfaceLight(const Vector2f& sample, const SurfaceInteraction& ref) const
{
    return SampleSurface(sample);
}

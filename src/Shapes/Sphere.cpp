#include "Sphere.h"
#include <glm/gtx/transform.hpp>
#include <SJson/SJson.h>
#include <Loaders/JsonLoader.h>
#include <Core/Utils.h>

std::shared_ptr<Shape> Sphere::CreateSphere(JsonNode_CPTR pShapeNode) {
    auto pos = loader::parse_vec3(pShapeNode->GetMember("Position"));
    auto r = pShapeNode->GetMember("Radius")->GetFloat();
    return std::make_shared<Sphere>(pos, r, glm::vec3(0));
}

Sphere::Sphere(glm::vec3 pos, 
    float radius, glm::vec3 rotation) 
    : _pos(pos), _radius(radius), _rot(rotation) {

    glm::mat4 rotMatrix = glm::identity<glm::mat4>();
    rotMatrix = glm::rotate(rotMatrix, rotation.x, glm::vec3(1, 0, 0));
    rotMatrix = glm::rotate(rotMatrix, rotation.y, glm::vec3(0, 1, 0));
    rotMatrix = glm::rotate(rotMatrix, rotation.z, glm::vec3(0, 0, 1));

    _local2World = glm::mat3(rotMatrix);
    _world2Local = glm::transpose(_local2World);
}

Sphere::~Sphere() {
}

BoundingBox Sphere::GetBoundingBox() const {
    return BoundingBox(glm::vec3(_pos - _radius), glm::vec3(_pos + _radius));
}

bool Sphere::Intersect(const Ray& ray, SurfaceInteraction* info) const {
    glm::vec3 P = _world2Local * (ray.start - _pos);
    glm::vec3 d = _world2Local * ray.dir;
    float a = glm::dot(d, d);
    float b = 2 * glm::dot(d, P);
    float c = glm::dot(P, P) - _radius * _radius;
    float discrim = b * b - 4 * a * c;
    if (discrim < 0) return false;
    discrim = sqrt(discrim);
    float t1 = (-b + discrim) / (2 * a);
    float t2 = (-b - discrim) / (2 * a);
    if (t1 > t2) std::swap(t1, t2);
    float t = t1;
    if (t1 < 0) t = t2;
    if (t < 0) return false;

    // Calculate local hit info, normal, front face, etc..
    auto dummyHitPos = P + d * t;
    auto N = glm::normalize(dummyHitPos);
    auto theta = std::atan2(-dummyHitPos.z, dummyHitPos.x) / glm::pi<float>();
    auto phi = std::acos(dummyHitPos.y / _radius) / glm::pi<float>() + 1.0f;

    auto front_face = glm::dot(d, N) < 0;
    N = front_face ? N : -N;

    auto realHitPos = _local2World * dummyHitPos + _pos;
    auto dpdu = glm::normalize(_local2World * glm::vec3(-N.z, 0, N.x));
    N = _local2World * N;
    info->SetHitInfo(t, realHitPos, ray.dir, N, glm::vec2(theta, phi), front_face, dpdu, glm::cross(N, dpdu));
    return true;
}

bool Sphere::IntersectTest(const Ray& ray, float tMin, float tMax) const {
    if (tMin > tMax) return false;
    glm::vec3 P = _world2Local * (ray.start - _pos);
    glm::vec3 d = _world2Local * ray.dir;
    float a = glm::dot(d, d);
    float b = 2 * glm::dot(d, P);
    float c = glm::dot(P, P) - _radius * _radius;
    float discrim = b * b - 4 * a * c;
    if (discrim < 0) return false;
    discrim = sqrt(discrim);
    float t1 = (-b + discrim) / (2 * a);
    float t2 = (-b - discrim) / (2 * a);
    return (t1 >= tMin && t1 <= tMax) || (t2 >= tMin && t2 <= tMax);
}

float Sphere::SurfaceArea() const
{
    return 4 * glm::pi<float>() * _radius * _radius;
}

Point3f Sphere::SamplePos(const Vector2f& sample, float& pdf) const
{
    pdf = 1.f / SurfaceArea();
    float p;
    return NextUnitSphere(sample, p) * _radius;
}

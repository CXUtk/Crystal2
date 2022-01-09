#pragma once
#include <glm/glm.hpp>
#include <algorithm>

struct Ray {
public:
    Ray() { start = glm::vec3(0), dir = glm::vec3(0); }
    Ray(glm::vec3 start, glm::vec3 dir) :start(start), dir(dir) {}
    glm::vec3 start, dir;
};

// Represents the bounding box of a geometry object
class BoundingBox {
public:
    BoundingBox();
    BoundingBox(const glm::vec3& minPos, const glm::vec3& maxPos);
    ~BoundingBox();

    glm::vec3 GetMinPos() const { return _minPos; }
    glm::vec3 GetMaxPos() const { return _maxPos; }

    bool RayIntersects(const Ray& ray, float& tMin, float& tMax) const;

    bool Intersects(const BoundingBox& box) const;

    glm::vec3 GetCenter() const { return (_minPos + _maxPos) / 2.f; }

    BoundingBox Union(const BoundingBox& box) const;

    BoundingBox Union(const glm::vec3& pos) const;

    BoundingBox IntersectWith(const BoundingBox& box) const;

    int MaxExtent() const;

    float SurfaceArea() const;

private:
    glm::vec3 _minPos, _maxPos;
};

inline bool RayBoxTest(const Ray& ray, bool inv[3], const glm::vec3& invD, 
    const BoundingBox& box, float& tMin, float& tMax)
{
    auto minP = (box.GetMinPos() - ray.start) * invD;
    auto maxP = (box.GetMaxPos() - ray.start) * invD;
    if (inv[0]) std::swap(minP[0], maxP[0]);
    tMin = std::max(tMin, minP[0]);
    tMax = std::min(tMax, maxP[0]);
    if (tMax < tMin) return false;

    if (inv[1]) std::swap(minP[1], maxP[1]);
    tMin = std::max(tMin, minP[1]);
    tMax = std::min(tMax, maxP[1]);
    if (tMax < tMin) return false;

    if (inv[2]) std::swap(minP[2], maxP[2]);
    tMin = std::max(tMin, minP[2]);
    tMax = std::min(tMax, maxP[2]);
    if (tMax < tMin) return false;
    return true;
}

inline bool RayBoxTest(const Ray& ray, const BoundingBox& box, float& tMin, float& tMax) {
    bool inv[3] = { ray.dir[0] < 0, ray.dir[1] < 0 , ray.dir[2] < 0 };
    glm::vec3 invD = { 1.0f / ray.dir[0], 1.0f / ray.dir[1], 1.0f / ray.dir[2] };
    auto minP = (box.GetMinPos() - ray.start) * invD;
    auto maxP = (box.GetMaxPos() - ray.start) * invD;
    for (int i = 0; i < 3; i++) {
        if (inv[i]) std::swap(minP[i], maxP[i]);
        tMin = std::max(tMin, minP[i]);
        tMax = std::min(tMax, maxP[i]);
        if (tMax < tMin) return false;
    }
    return true;
}

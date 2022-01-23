#pragma once
#include <glm/glm.hpp>
#include <algorithm>

template<glm::length_t L, typename T, glm::qualifier Q>
struct Ray {
public:
    using Vec = glm::vec<L, T, Q>;
    Ray() { start = Vec(0), dir = Vec(0); }
    Ray(const Vec& start, const Vec& dir) :start(start), dir(dir) {}
    Vec start, dir;
};

// Represents the bounding box of a geometry object
template<glm::length_t L, typename T, glm::qualifier Q>
class BoundingBox
{
public:
    using Vec = glm::vec<L, T, Q>;
    using BBox = BoundingBox<L, T, Q>;

    BoundingBox() :_minPos(Vec(std::numeric_limits<float>::infinity())),
        _maxPos(Vec(-std::numeric_limits<float>::infinity()))
    {}
    BoundingBox(const Vec& minPos, const Vec& maxPos)

        : _minPos(minPos), _maxPos(maxPos)
    {}
    ~BoundingBox() {}

    Vec GetMinPos() const { return _minPos; }
    Vec GetMaxPos() const { return _maxPos; }

    bool RayIntersects(const Ray<L, T, Q>& ray, float& tMin, float& tMax) const
    {
        Vec start = ray.start, dir = ray.dir;
        for (int i = 0; i < L; i++)
        {
            auto invD = (float)1 / dir[i];
            auto t0 = (_minPos[i] - start[i]) * invD;
            auto t1 = (_maxPos[i] - start[i]) * invD;
            if (invD < 0) std::swap(t0, t1);
            tMin = std::max(tMin, t0);
            tMax = std::min(tMax, t1);
            if (tMax < tMin)
            {
                return false;
            }
        }
        return true;
    }

    bool Intersects(const BBox& box) const
    {
        for (int i = 0; i < L; i++)
        {
            if (std::max(_minPos[i], box.GetMinPos()[i]) > std::min(_maxPos[i], box.GetMaxPos()[i]))
            {
                return false;
            }
        }
        return true;
    }

    Vec GetCenter() const { return (_minPos + _maxPos) / 2.f; }

    BBox Union(const BBox& box) const
    {
        Vec minn = glm::min(_minPos, box._minPos);
        Vec maxx = glm::max(_maxPos, box._maxPos);
        return BBox(minn, maxx);
    }

    BBox Union(const Vec& pos) const
    {
        Vec minn = glm::min(_minPos, pos);
        Vec maxx = glm::max(_maxPos, pos);
        return BBox(minn, maxx);
    }

    BBox IntersectWith(const BBox& box) const
    {
        Vec minn = glm::max(_minPos, box._minPos);
        Vec maxx = glm::min(_maxPos, box._maxPos);
        return BBox(minn, maxx);
    }

    int MaxExtent() const
    {
        auto v = _maxPos - _minPos;
        int maxAxis = 0;
        auto d = std::abs(v[0]);
        for (int i = 1; i < L; i++)
        {
            if (std::abs(v[i]) > d)
            {
                d = std::abs(v[i]);
                maxAxis = i;
            }
        }
        return maxAxis;
    }

    float SurfaceArea() const
    {
        float area = 0;
        for (int i = 0; i < L; i++)
        {
            area += 2 * (_maxPos[i] - _minPos[i]) * (_maxPos[(i + 1) % L] - _minPos[(i + 1) % L]);
        }
        return area;
    }

private:
    Vec _minPos, _maxPos;
};

template<glm::length_t L, typename T, glm::qualifier Q>
inline bool RayBoxTest(const Ray<L, T, Q>& ray, const BoundingBox<L, T, Q>& box, float& tMin, float& tMax)
{
    bool inv[3] = { ray.dir[0] < 0, ray.dir[1] < 0 , ray.dir[2] < 0 };
    glm::vec3 invD = { 1.0f / ray.dir[0], 1.0f / ray.dir[1], 1.0f / ray.dir[2] };
    auto minP = (box.GetMinPos() - ray.start) * invD;
    auto maxP = (box.GetMaxPos() - ray.start) * invD;
    for (int i = 0; i < 3; i++)
    {
        if (inv[i]) std::swap(minP[i], maxP[i]);
        tMin = std::max(tMin, minP[i]);
        tMax = std::min(tMax, maxP[i]);
        if (tMax < tMin) return false;
    }
    return true;
}


template<typename T, glm::qualifier Q>
inline bool RayBoxTest(const Ray<3, T, Q>& ray, bool inv[3], const glm::vec<3, T, Q>& invD,
    const BoundingBox<3, T, Q>& box, float& tMin, float& tMax)
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

using Ray3f = Ray<3, float, glm::qualifier::defaultp>;
using Ray2f = Ray<2, float, glm::qualifier::defaultp>;

using Bound3f = BoundingBox<3, float, glm::qualifier::defaultp>;
using Bound2f = BoundingBox<2, float, glm::qualifier::defaultp>;
using Bound3i = BoundingBox<3, int, glm::qualifier::defaultp>;
using Bound2i = BoundingBox<2, int, glm::qualifier::defaultp>;
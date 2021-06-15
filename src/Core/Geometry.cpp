#include "Geometry.h"
#include <algorithm>

BoundingBox::BoundingBox() : _minPos(glm::vec3(std::numeric_limits<float>::infinity())), _maxPos(glm::vec3(-std::numeric_limits<float>::infinity())) {
}

BoundingBox::BoundingBox(const glm::vec3& minPos, const glm::vec3& maxPos) : _minPos(minPos), _maxPos(maxPos) {
}

BoundingBox::~BoundingBox() {
}

bool BoundingBox::RayIntersects(const Ray& ray, float& tMin, float& tMax) const {
    glm::vec3 start = ray.start, dir = ray.dir;
    for (int i = 0; i < 3; i++) {
        auto invD = 1.0f / dir[i];
        auto t0 = (_minPos[i] - start[i]) * invD;
        auto t1 = (_maxPos[i] - start[i]) * invD;
        if (invD < 0) std::swap(t0, t1);
        tMin = std::max(tMin, t0);
        tMax = std::min(tMax, t1);
        if (tMax < tMin) {
            return false;
        }
    }
    return true;
}

bool BoundingBox::Intersects(const BoundingBox& box) const {
    for (int i = 0; i < 3; i++)
        if (std::max(_minPos[i], box.GetMinPos()[i]) > std::min(_maxPos[i], box.GetMaxPos()[i])) return false;
    return true;
}

BoundingBox BoundingBox::Union(const BoundingBox& box) const {
    glm::vec3 minn(std::min(_minPos.x, box._minPos.x), std::min(_minPos.y, box._minPos.y), std::min(_minPos.z, box._minPos.z));
    glm::vec3 maxx(std::max(_maxPos.x, box._maxPos.x), std::max(_maxPos.y, box._maxPos.y), std::max(_maxPos.z, box._maxPos.z));
    return BoundingBox(minn, maxx);
}

BoundingBox BoundingBox::Union(const glm::vec3& pos) const {
    glm::vec3 minn(std::min(_minPos.x, pos.x), std::min(_minPos.y, pos.y), std::min(_minPos.z, pos.z));
    glm::vec3 maxx(std::max(_maxPos.x, pos.x), std::max(_maxPos.y, pos.y), std::max(_maxPos.z, pos.z));
    return BoundingBox(minn, maxx);
}

BoundingBox BoundingBox::IntersectWith(const BoundingBox& box) const {
    glm::vec3 minn(std::max(_minPos.x, box._minPos.x), std::max(_minPos.y, box._minPos.y), std::max(_minPos.z, box._minPos.z));
    glm::vec3 maxx(std::min(_maxPos.x, box._maxPos.x), std::min(_maxPos.y, box._maxPos.y), std::min(_maxPos.z, box._maxPos.z));
    return BoundingBox(minn, maxx);
}

int BoundingBox::MaxExtent() const {
    int t = _maxPos[0] - _minPos[0], d = 0;
    if (_maxPos[1] - _minPos[1] > t) t = _maxPos[1] - _minPos[1], d = 1;
    if (_maxPos[2] - _minPos[2] > t) d = 2;
    return d;
}

float BoundingBox::SurfaceArea() const {
    float area = 0;
    for (int i = 0; i < 3; i++)
        area += 2 * (_maxPos[i] - _minPos[i]) * (_maxPos[(i + 1) % 3] - _minPos[(i + 1) % 3]);
    return area;
}

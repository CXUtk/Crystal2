#include "Brute.h"

Brute::Brute() {
}

Brute::~Brute() {
}

void Brute::Build(const std::vector<std::shared_ptr<Shape>>& objects) {
    for (auto& ptr : objects) {
        _objects.push_back(ptr);
    }
}

bool Brute::Intersect(const Ray& ray, SurfaceInteraction* isec) const {
    bool hit = false;
    int cnt = 0;
    for (auto& obj : _objects) {
        SurfaceInteraction tmp;
        if (obj->Intersect(ray, &tmp)) {
            if (tmp.GetDistance() < isec->GetDistance()) {
                memcpy(isec, &tmp, sizeof(SurfaceInteraction));
            }
            hit = true;
            if (isec->GetDistance() == std::numeric_limits<float>::infinity()) {
                hit = false;
            }
        }
    }

    return hit;
}

bool Brute::IntersectTest(const Ray& ray, float tMin, float tMax) const {
    for (auto& obj : _objects) {
        SurfaceInteraction tmp;
        if (obj->IntersectTest(ray, tMin, tMax)) {
            return true;
        }
    }
    return false;
}


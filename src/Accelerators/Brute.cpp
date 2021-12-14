#include "Brute.h"
#include <Core/SurfaceInteraction.h>
#include <Core/Entities.h>

Brute::Brute() {
}

Brute::~Brute() {
}

void Brute::Build(const std::vector<const crystal::Entity*>& objects)
{
    for (auto ptr : objects)
    {
        _entities.push_back(ptr);
    }
}

bool Brute::Intersect(const Ray& ray, SurfaceInteraction* isec, float tMin, float tMax) const
{
    bool hit = false;
    int cnt = 0;
    for (auto& obj : _entities)
    {
        SurfaceInteraction tmp;
        if (obj->Intersect(ray, &tmp))
        {
            auto dist = tmp.GetDistance();
            if (dist < isec->GetDistance())
            {
                tmp.SetHitEntity(obj);
                *isec = std::move(tmp);
            }
            hit = true;
        }
    }
    return hit;
}


bool Brute::IntersectTest(const Ray& ray, float tMin, float tMax) const
{
    for (auto& obj : _entities)
    {
        if (obj->IntersectTest(ray, tMin, tMax))
        {
            return true;
        }
    }
    return false;
}
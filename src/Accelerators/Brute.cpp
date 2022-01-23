#include "Brute.h"
#include <Core/SurfaceInteraction.h>
#include <Core/Entities.h>

Brute::Brute() :Accelerator()
{}

Brute::~Brute()
{}

void Brute::Build(const std::vector<const crystal::IIntersectable*>& objects)
{
	for (auto ptr : objects)
	{
		_objects.push_back(ptr);
	}
}

bool Brute::Intersect(const Ray3f& ray, SurfaceInteraction* isec,
	float tMin, float tMax) const
{
	bool hit = false;
	int cnt = 0;
	for (auto& obj : _objects)
	{
		SurfaceInteraction tmp;
		if (obj->Intersect(ray, &tmp))
		{
			auto dist = tmp.GetDistance();
			if (dist < isec->GetDistance())
			{
				tmp.SetHitEntity(dynamic_cast<const crystal::Entity*>(obj));

				*isec = std::move(tmp);
			}
			hit = true;
		}
	}
	return hit;
}


bool Brute::IntersectTest(const Ray3f& ray, const crystal::IIntersectable* ignoreShape,
	float tMin, float tMax) const
{
	for (auto& obj : _objects)
	{
		if (obj == ignoreShape) continue;
		if (obj->IntersectTest(ray, ignoreShape, tMin, tMax))
		{
			return true;
		}
	}
	return false;
}
#pragma once
#include <memory>
#include <string>
#include <vector>
#include <Crystal2.h>
#include <Core/Geometry.h>

class Accelerator
{
public:
	static std::unique_ptr<Accelerator> GetAccelerator(const std::string& name, bool shouldSetHitEntity);

	Accelerator(bool shouldSetHitEntity) : _shouldSetHitEntity(shouldSetHitEntity) {}
	virtual ~Accelerator() = 0 {}

	virtual void Build(const std::vector<const crystal::IIntersectable*>& objects) = 0;
	virtual bool Intersect(const Ray& ray, SurfaceInteraction* info,
		float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const = 0;
	virtual bool IntersectTest(const Ray& ray, float tMin = 0,
		float tMax = std::numeric_limits<float>::infinity()) const = 0;

protected:
	bool _shouldSetHitEntity;
};

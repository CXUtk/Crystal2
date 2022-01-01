#include "Shape.h"
#include <Core/Utils.h>

float Shape::PdfLight(const SurfaceInteraction& ref, const Vector3f& wi) const
{
	Ray ray = ref.SpawnRay(wi);
	SurfaceInteraction isec;
	if (!Intersect(ray, &isec)) return 0.f;

	return sqr(isec.GetHitPos() - ref.GetHitPos()) 
		/ (std::max(0.f, glm::dot(-wi, isec.GetNormal())) * SurfaceArea());
}

#include "DiffusedAreaLight.h"
#include <glm/gtx/transform.hpp>
#include <Shapes/Shape.h>

crystal::DiffusedAreaLight::DiffusedAreaLight(const Shape* shape, Spectrum flux, int numSamples)
	: AreaLight(numSamples), _shape(shape), _flux(flux)
{
	_area = _shape->SurfaceArea();
}

Spectrum crystal::DiffusedAreaLight::EvalEmission(const SurfaceInteraction& isec, const Vector3f& w) const
{
	if (glm::dot(w, isec.GetNormal()) > 0)
	{
		return _flux * normalizeFactor();
	}
	else
	{
		return Spectrum(0.f);
	}
}

Spectrum crystal::DiffusedAreaLight::Sample_Li(const SurfaceInteraction& hit, const glm::vec2& sample, glm::vec3* endpoint, float* pdf) const
{
	*endpoint = _shape->SamplePos(sample, *pdf);
	auto dir = glm::normalize(hit.GetHitPos() - *endpoint);
	return EvalEmission(hit, dir);
}

float crystal::DiffusedAreaLight::normalizeFactor() const
{
	return 1.f / glm::pi<float>() / _area;
}

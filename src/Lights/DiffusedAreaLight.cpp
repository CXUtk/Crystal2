#include "DiffusedAreaLight.h"
#include <glm/gtx/transform.hpp>
#include <Shapes/Shape.h>

crystal::DiffusedAreaLight::DiffusedAreaLight(const Shape* shape, 
	Spectrum flux, int numSamples)
	: AreaLight(numSamples, shape), _shape(shape), _flux(flux)
{
	_area = _shape->SurfaceArea();
}

Spectrum crystal::DiffusedAreaLight::EvalEmission(const SurfaceInfo& surface, const Vector3f& w) const
{
	if (glm::dot(surface.GetNormal(), w) < 0)
	{
		return Spectrum(0.f);
	}
	else
	{
		return _flux * normalizeFactor();
	}
}

Spectrum crystal::DiffusedAreaLight::Sample_Li(const SurfaceInteraction& hit, const glm::vec2& sample, glm::vec3* endpoint, float* pdf) const
{
	auto surface = _shape->SampleSurface(sample, pdf);
	*endpoint = surface.GetPosition();
	auto dir = glm::normalize(hit.GetHitPos() - *endpoint);
	return EvalEmission(surface, dir);
}

float crystal::DiffusedAreaLight::normalizeFactor() const
{
	return 1.f / glm::pi<float>() / _area;
}

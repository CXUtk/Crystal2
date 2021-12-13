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
	return _flux * normalizeFactor();
}

glm::vec3 crystal::DiffusedAreaLight::Sample_Li(const SurfaceInteraction& hit, const glm::vec2& sample, glm::vec3* endpoint, float* pdf) const
{
	return glm::vec3();
}

float crystal::DiffusedAreaLight::normalizeFactor() const
{
	return 1.f / glm::pi<float>() / _area;
}

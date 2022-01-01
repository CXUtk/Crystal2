#include "DiffusedAreaLight.h"
#include <glm/gtx/transform.hpp>
#include <Shapes/Shape.h>
#include <Core/Utils.h>

crystal::DiffusedAreaLight::DiffusedAreaLight(const Shape* shape, 
	const Spectrum& emitRadiance, int numSamples)
	: AreaLight(numSamples, shape), _shape(shape), _emitRadiance(emitRadiance)
{
	_area = _shape->SurfaceArea();
}

Spectrum crystal::DiffusedAreaLight::EvalEmitRadiance(const SurfaceInfo& surface, const Vector3f& w) const
{
	if (glm::dot(surface.GetNormal(), w) < 0)
	{
		return Spectrum(0.f);
	}
	else
	{
		return _emitRadiance;
	}
}

Spectrum crystal::DiffusedAreaLight::Sample_Li(const SurfaceInteraction& hit, 
	const glm::vec2& sample, glm::vec3* endpoint, float* pdf) const
{
	auto surface = _shape->SampleSurface(sample);
	*endpoint = surface.GetPosition();
	*pdf = _shape->Pdf(surface);

	auto lvec = hit.GetHitPos() - *endpoint;
	auto dir = glm::normalize(lvec);
	return EvalEmitRadiance(surface, dir) / sqr(lvec) * std::max(0.f, glm::dot(surface.GetNormal(), dir));
}

float crystal::DiffusedAreaLight::Pdf_Li(const SurfaceInfo& surface, const Vector3f& wi) const
{
	return 0.0f;
}

Spectrum crystal::DiffusedAreaLight::Flux() const
{
	return _emitRadiance * glm::pi<float>() * _area;
}

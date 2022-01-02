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

Spectrum crystal::DiffusedAreaLight::EvalEmitRadiance(const SurfaceInfo& surface_l, const Vector3f& w) const
{
	if (glm::dot(surface_l.GetNormal(), w) < 0)
	{
		return Spectrum(0.f);
	}
	else
	{
		return _emitRadiance;
	}
}

Spectrum crystal::DiffusedAreaLight::Sample_Li(const SurfaceInfo& surface_w,
	const glm::vec2& sample, glm::vec3* endpoint, float* pdf) const
{
	auto surface_light = _shape->SampleSurface(sample);
	*endpoint = surface_light.GetPosition();

	auto dir = glm::normalize(*endpoint - surface_w.GetPosition());
	*pdf = _shape->PdfLight(surface_w, dir);
	return EvalEmitRadiance(surface_light, -dir);
}

float crystal::DiffusedAreaLight::Pdf_Li(const SurfaceInfo& surface_w, const Vector3f& wi) const
{
	return _shape->PdfLight(surface_w, wi);
}

Spectrum crystal::DiffusedAreaLight::Flux() const
{
	return _emitRadiance * glm::pi<float>() * _area;
}

#include "DiffuseMaterial.h"
#include <Core/SurfaceInteraction.h>
#include <BSDFs/Lambertain.h>
#include <BSDFs/Reflections.h>
#include <algorithm>
#include <cmath>
#include <Textures/Texture.h>


DiffuseMaterial::DiffuseMaterial(const Texture_RGB* Kd) : _Kd(Kd)
{}

DiffuseMaterial::~DiffuseMaterial()
{}

void DiffuseMaterial::ComputeScatteringFunctions(SurfaceInteraction& isec, bool fromCamera) const
{
	auto color = _Kd->Evaluate(isec);
	isec.GetBSDF()->AddBxDF(std::make_shared<Lambertain>(color));
}

OrenNayarMaterial::OrenNayarMaterial(const Texture_RGB* Kd, const Texture_Float* sigma)
	: _Kd(Kd), _sigma(sigma)
{}

OrenNayarMaterial::~OrenNayarMaterial()
{}

void OrenNayarMaterial::ComputeScatteringFunctions(SurfaceInteraction& isec, bool fromCamera) const
{
	auto color = _Kd->Evaluate(isec);
	auto sigma = _sigma->Evaluate(isec);
	isec.GetBSDF()->AddBxDF(std::make_shared<crystal::OrenNayarReflection>(color, sigma));
}

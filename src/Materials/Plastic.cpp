#include "Plastic.h"
#include <BSDFs/FresnelBlend.h>
#include <Core/SurfaceInteraction.h>

Plastic::Plastic(const Spectrum& kd, const Spectrum& ks, Vector2f roughness, NDFType ndf)
	: _kd(kd), _ks(ks), _roughness(roughness), _ndfType(ndf)
{}

Plastic::~Plastic()
{}

void Plastic::ComputeScatteringFunctions(SurfaceInteraction & isec, bool fromCamera) const
{
    std::shared_ptr<MicrofacetDistribution> distribution = nullptr;
    switch (_ndfType)
    {
    case NDFType::Beckmann:
        distribution = std::make_shared<BeckmannDistribution>(_roughness);
        break;
    case NDFType::GGX:
        distribution = std::make_shared<GGXDistribution>(_roughness);
        break;
    default:
        break;
    }
	isec.GetBSDF()->AddBxDF(std::make_shared<FresnelBlend>(_kd, 
        _ks, distribution));
}

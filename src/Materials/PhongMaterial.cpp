#include "PhongMaterial.h"
#include <Core/SurfaceInteraction.h>
#include <Textures/Texture.h>
#include <BSDFs/BlinnPhongReflection.h>
#include <BSDFs/Lambertain.h>

crystal::PhongMaterial::PhongMaterial(const Texture_RGB* Kd, int K) : _Kd(Kd), _K(K)
{}

crystal::PhongMaterial::~PhongMaterial()
{}

void crystal::PhongMaterial::ComputeScatteringFunctions(SurfaceInteraction& isec,
    bool fromCamera) const
{
    auto bsdf = std::make_shared<BSDF>(&isec);

    auto color = _Kd->Evaluate(isec);

    //bsdf->AddBxDF(std::make_shared<Lambertain>(glm::vec3(0.5), glm::mat3(T, N, B)));
    isec.GetBSDF()->AddBxDF(std::make_shared<BlinnPhongReflection>(color, _K));
}

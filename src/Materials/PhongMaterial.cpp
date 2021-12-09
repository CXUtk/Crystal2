#include "PhongMaterial.h"
#include <Core/SurfaceInteraction.h>
#include <Textures/Texture.h>
#include <BSDFs/BlinnPhongReflection.h>
#include <BSDFs/Lambertain.h>

crystal::PhongMaterial::PhongMaterial(const Texture_RGB* Kd, int K) : _Kd(Kd), _K(K)
{}

crystal::PhongMaterial::~PhongMaterial()
{}

std::shared_ptr<BSDF> crystal::PhongMaterial::ComputeScatteringFunctions(const SurfaceInteraction& isec,
    bool fromCamera) const
{
    auto N = glm::normalize(isec.GetNormal());
    auto T = glm::normalize(isec.GetDpDu());
    auto B = glm::normalize(glm::cross(T, N));

    auto bsdf = std::make_shared<BSDF>(&isec);

    auto color = _Kd->Evaluate(isec);

    //bsdf->AddBxDF(std::make_shared<Lambertain>(glm::vec3(0.5), glm::mat3(T, N, B)));
    bsdf->AddBxDF(std::make_shared<BlinnPhongReflection>(color, glm::mat3(T, N, B), _K));
    return bsdf;
}

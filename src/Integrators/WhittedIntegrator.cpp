#include "WhittedIntegrator.h"
#include <Core/Scene.h>
#include <Core/Geometry.h>
#include <Core/SurfaceInteraction.h>
#include <glm/gtx/transform.hpp>
#include <Core/Prototype.h>
#include <BSDFs/BSDF.h>

static constexpr float pRR = 0.8f;
static constexpr float EPS = 1e-5;

glm::vec3 WhittedIntegrator::Evaluate(const Ray& ray, const std::shared_ptr<Scene>& scene,
    const std::shared_ptr<Sampler>& sampler) {
    return eval_rec(ray, scene, sampler, 0);
}


glm::vec3 WhittedIntegrator::eval_rec(const Ray& ray, const std::shared_ptr<Scene>& scene,
    const std::shared_ptr<Sampler>& sampler, int level) {
    if (level == 3) return glm::vec3(0);
    SurfaceInteraction info;
    if (scene->Intersect(ray, &info)) {
        glm::vec3 N = info.GetNormal();
        glm::vec3 hitPos = info.GetHitPos();
        auto prototype = info.GetHitPrototype();
        auto bsdf = prototype->ComputeScatteringFunctions(info);

        glm::vec3 wIn;
        float pdf;
        auto brdf = bsdf->SampleDirection(sampler->Get1D(), sampler->Get2D(), -ray.dir, &wIn, &pdf, BxDFType::BxDF_ALL);
        if (std::abs(pdf) < EPS) return glm::vec3(0);

        bool specular = (bsdf->Flags() & BxDF_SPECULAR) != 0;
        auto cosine = specular ? 1.0f : std::max(0.f, glm::dot(N, wIn));
        auto Li = eval_rec(info.SpawnRay(wIn), scene, sampler, level + 1) * brdf * cosine / pdf;
        return Li;
    }
    auto Lenvir = ray.dir.y * 0.5f + 0.5f;
    return glm::vec3(glm::smoothstep(0.f, 1.f, Lenvir));
}

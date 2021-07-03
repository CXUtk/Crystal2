#include "PathTracingIntegrator.h"
#include <Core/Scene.h>
#include <Core/Geometry.h>
#include <Core/SurfaceInteraction.h>
#include <glm/gtx/transform.hpp>
#include <Core/Prototype.h>
#include <BSDFs/BSDF.h>
#include <Samplers/DefaultSampler.h>
#include <Lights/Light.h>

static constexpr float pRR = 0.8f;
static constexpr float EPS = 1e-5;

PathTracingIntegrator::PathTracingIntegrator(const std::shared_ptr<Sampler>& sampler) : SamplerIntegrator(sampler) {
    _indirectSampler = std::make_shared<DefaultSampler>(sampler->GetSamplesPerPixel(), 0);
}


// 路径追踪渲染器一开始使用选定sampler，在经过一次transport以后变成随机sampler
glm::vec3 PathTracingIntegrator::Evaluate(const Ray& ray, const std::shared_ptr<Scene>& scene,
    const std::shared_ptr<Sampler>& sampler) {
    return eval_rec(ray, scene, sampler, 0);
}


glm::vec3 PathTracingIntegrator::eval_rec(const Ray& ray, const std::shared_ptr<Scene>& scene,
    const std::shared_ptr<Sampler>& sampler, int level) {
    if (level == 5) return glm::vec3(0);
    glm::vec3 Lres(0);
    SurfaceInteraction info;
    if (scene->Intersect(ray, &info)) {
        glm::vec3 N = info.GetNormal();
        glm::vec3 hitPos = info.GetHitPos();
        auto prototype = info.GetHitPrototype();
        auto bsdf = prototype->ComputeScatteringFunctions(info, ray.dir);

        // 如果不是纯光滑表面就计算从光源采样的irradiance
        if (!(bsdf->Flags() & BxDF_SPECULAR)) {
            for (auto& light : scene->GetLights()) {
                glm::vec3 endpoint;
                float pdf;
                auto Li = light->Sample_Li(info, sampler->Get2D(1), &endpoint, &pdf);
                SurfaceInteraction hit;

                auto LVec = endpoint - hitPos;
                auto LDir = glm::normalize(LVec);
                if (!scene->Intersect(info.SpawnRay(LDir), &hit) || hit.GetDistance() > glm::length(LVec)) {
                    Lres += bsdf->DistributionFunction(-ray.dir, LDir) * Li / glm::dot(LVec, LVec) * std::max(0.f, glm::dot(N, LDir));
                }
            }
        }

        // 进行一次路径追踪采样
        glm::vec3 wIn;
        float pdf;
        auto brdf = bsdf->SampleDirection(sampler->Get1D(1), sampler->Get2D(1), -ray.dir, &wIn, &pdf, BxDFType::BxDF_ALL);
        if (std::abs(pdf) < EPS) return glm::vec3(0);

        bool specular = (bsdf->Flags() & BxDF_SPECULAR) != 0;
        auto cosine = specular ? 1.0f : std::max(0.f, glm::dot(N, wIn));

        auto Lindir = eval_rec(info.SpawnRay(wIn), scene, _indirectSampler, level + 1) * brdf * cosine / pdf;
        Lres += Lindir;
        return Lres;
    }
    return Lres;
    auto Lenvir = ray.dir.y * 0.5f + 0.5f;
    return glm::vec3(glm::smoothstep(0.f, 1.f, Lenvir));
}

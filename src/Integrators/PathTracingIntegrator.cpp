#include "PathTracingIntegrator.h"

#include <Core/Scene.h>
#include <Core/Geometry.h>
#include <Core/SurfaceInteraction.h>
#include <Core/Prototype.h>

#include <glm/gtx/transform.hpp>
#include <BSDFs/BSDF.h>
#include <Samplers/DefaultSampler.h>
#include <Lights/Light.h>

#include <Textures/CubemapTexture.h>

static constexpr float pRR = 0.8f;
static constexpr float EPS = 1e-5;

PathTracingIntegrator::PathTracingIntegrator(const std::shared_ptr<Sampler>& sampler, int threads, int maxDepth)
    : SamplerIntegrator(sampler, threads), _maxDepth(maxDepth) {

}


// 路径追踪渲染器一开始使用选定sampler，在经过一次transport以后变成随机sampler
glm::vec3 PathTracingIntegrator::Evaluate(const Ray& ray, Scene* scene,
    Sampler* sampler) {
    return eval_rec(ray, scene, sampler, 0, true);
}


glm::vec3 PathTracingIntegrator::eval_rec(const Ray& ray, Scene* scene,
    Sampler* sampler, int level, bool specular) {
    if (level == _maxDepth) return glm::vec3(0.0f);
    glm::vec3 Lres(0);
    SurfaceInteraction info;
    if (scene->Intersect(ray, &info)) {
        glm::vec3 N = info.GetNormal();
        glm::vec3 hitPos = info.GetHitPos();
        auto prototype = info.GetHitPrototype();
        auto bsdf = prototype->ComputeScatteringFunctions(info, ray.dir);

        // 如果是自发光物体就把发光项加上
        if (info.GetHitPrototype()->GetAreaLight() != nullptr && specular) {
            auto areaLight = info.GetHitPrototype()->GetAreaLight();
            Lres += areaLight->EvalEmission(info);
        }

        // 计算从光源采样的radiance
        for (auto& light : scene->GetLights()) {
            glm::vec3 endpoint;
            float pdf;
            auto Li = light->Sample_Li(info, sampler->Get2D(1), &endpoint, &pdf);
            SurfaceInteraction hit;

            auto LVec = endpoint - hitPos;
            auto LDir = glm::normalize(LVec);
            if (!scene->IntersectTest(info.SpawnRay(LDir), 0, glm::length(LVec) - EPS)) {
                Lres += bsdf->DistributionFunction(-ray.dir, LDir) * Li / glm::dot(LVec, LVec) * std::max(0.f, glm::dot(N, LDir)) / pdf;
            }
        }

        // 进行一次路径追踪采样
        glm::vec3 wIn;
        float pdf;
        BxDFType type;
        auto brdf = bsdf->SampleDirection(sampler->Get1D(1), sampler->Get2D(1), -ray.dir, &wIn, &pdf, BxDFType::BxDF_ALL, &type);
        if (std::abs(pdf) < EPS || brdf == glm::vec3(0)) return Lres;
        bool specular = (type & BxDF_SPECULAR) != 0;

        auto cosine = specular ? 1.0f : std::max(0.f, glm::dot(N, wIn));
        auto Lindir = eval_rec(info.SpawnRay(wIn), scene, sampler, level + 1, specular) * brdf * cosine / pdf;
        Lres += Lindir;
        return Lres;
    }
    return glm::vec3(0.5f);
    if (scene->GetSkybox() == nullptr) return glm::vec3(0.f);
    return scene->GetSkybox()->Evaluate(ray.dir);
}

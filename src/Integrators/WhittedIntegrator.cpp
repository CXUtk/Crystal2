#include "WhittedIntegrator.h"
#include <Core/Scene.h>
#include <Core/Geometry.h>
#include <Core/SurfaceInteraction.h>
#include <glm/gtx/transform.hpp>

glm::vec3 WhittedIntegrator::Evaluate(const Ray& ray, const std::shared_ptr<Scene>& scene,
    const std::shared_ptr<Sampler>& sampler) {
    return eval_rec(ray, scene, sampler, 0);
}

glm::vec3 NextCosineUnitHemiSphere(glm::vec2 sample, float& pdf) {
    auto r = std::sqrt(sample.x);
    auto phi = sample.y * glm::two_pi<float>();

    auto x = r * std::cos(phi);
    auto z = r * std::sin(phi);
    auto y = std::sqrt(1.0f - r);
    pdf = y / glm::pi<float>();
    return glm::vec3(x, y, z);
}

glm::vec3 NextUnitHemiSphere(glm::vec2 sample, float& pdf) {
    auto y = sample.x;
    auto phi = sample.y * glm::two_pi<float>();

    float r = std::sqrt(1 - y * y);
    auto x = r * std::cos(phi);
    auto z = r * std::sin(phi);

    pdf = 1.0f / glm::two_pi<float>();
    return glm::vec3(x, y, z);
}

glm::vec3 WhittedIntegrator::eval_rec(const Ray& ray, const std::shared_ptr<Scene>& scene,
    const std::shared_ptr<Sampler>& sampler, int level) {
    if (level == 3) return glm::vec3(0);
    SurfaceInteraction info;
    if (scene->Intersect(ray, &info)) {
        auto N = glm::normalize(info.GetNormal());
        auto T = glm::normalize(info.GetDpDu());
        auto B = glm::normalize(info.GetDpDv());

        float pdf;
        auto wo = NextUnitHemiSphere(sampler->Get2D(), pdf);
        auto cosine = wo.y;
        wo = wo.x * T + wo.y * N + wo.z * B;

        auto Li = eval_rec(info.SpawnRay(wo), scene, sampler, level + 1) * cosine / glm::pi<float>() / pdf;
        return Li;
    }
    auto Lenvir = ray.dir.y * 0.5f + 0.5f;
    return glm::vec3(0.5f);
}

#include "WhittedIntegrator.h"
#include <Core/Scene.h>
#include <Core/Geometry.h>
#include <Core/SurfaceInteraction.h>
#include <glm/gtx/transform.hpp>

glm::vec3 WhittedIntegrator::Evaluate(const Ray& ray, const std::shared_ptr<Scene>& scene, 
    const std::shared_ptr<Sampler>& sampler) {
    return eval_rec(ray, scene, sampler, 0);
}

glm::vec3 NextCosineUnitHemiSphere(glm::vec2 sample) {
    auto r = std::sqrt(sample.x);
    auto theta = sample.y * glm::two_pi<float>();

    auto x = r * std::cos(theta);
    auto z = r * std::sin(theta);
    return glm::vec3(x, std::sqrt(1 - r), z);
}

glm::vec3 WhittedIntegrator::eval_rec(const Ray& ray, const std::shared_ptr<Scene>& scene, 
    const std::shared_ptr<Sampler>& sampler, int level) {
    if (level == 3) return glm::vec3(0);
    SurfaceInteraction info;
    if (scene->Intersect(ray, &info)) {
        auto N = glm::normalize(info.GetNormal());
        auto T = glm::normalize(info.GetDpDu());
        auto B = glm::cross(N, T);

        auto wo = NextCosineUnitHemiSphere(sampler->Get2D());
        wo = wo.x * T + wo.y * N + wo.z * B;

        auto Li = eval_rec(info.SpawnRay(wo), scene, sampler, level + 1) / glm::pi<float>();
        return Li;
    }

    auto Lenvir = ray.dir.y * 0.5f + 0.5f;
   return glm::vec3(glm::smoothstep(0.f, 1.f, Lenvir));
}

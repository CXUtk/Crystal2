#include "WhittedIntegrator.h"
#include <Core/Scene.h>
#include <Core/Geometry.h>
#include <Core/SurfaceInteraction.h>

glm::vec3 WhittedIntegrator::Evaluate(const Ray& ray, const std::shared_ptr<Scene>& scene) {
    SurfaceInteraction info;
    if (scene->Intersect(ray, &info)) {
        return glm::vec3(1.f);
    }
    return glm::vec3(ray.dir.y);
}

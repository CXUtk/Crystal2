#pragma once
#include <Crystal2.h>
#include <glm/glm.hpp>

class Light {
public:
    Light() {}
    virtual ~Light() = 0 {}

    virtual glm::vec3 Flux() const = 0;

    // Get the radiance value from the light to the object surface hit point
    virtual glm::vec3 Sample_Li(const SurfaceInteraction& hit, const glm::vec2& sample, glm::vec3* endpoint, float* pdf) const = 0;
    //virtual glm::vec3 SampleRadiance(glm::vec3 dir) const = 0;
    //virtual glm::vec3 SampleEmission(glm::vec3* pos, glm::vec3* dir, float* pdf) const = 0;
};

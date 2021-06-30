#pragma once
#include <Crystal2.h>
class Light {
public:
    Light() {}
    virtual ~Light() = 0 {}

    // Get the radiance value based on the hit point
    virtual glm::vec3 SampleLi(const SurfaceInteraction& hit, glm::vec3& endpoint, float* pdf) const = 0 {}
    virtual float GetArea() const { return 0; }
    virtual glm::vec3 SampleRadiance(glm::vec3 dir) const = 0;

    virtual glm::vec3 SampleEmission(glm::vec3* pos, glm::vec3* dir, float* pdf) const = 0;
};

#pragma once
#include <glm/glm.hpp>
#include "Geometry.h"

class SurfaceInteraction {
public:
    SurfaceInteraction();

    Ray SpawnRay(glm::vec3 dir, bool pass = false) const;
    void SetHitInfo(float t, glm::vec3 hitPos, glm::vec3 normal, glm::vec2 uv,
        bool frontFace, const Shape* shape, glm::vec3 dpdu, glm::vec3 dpdv);
};
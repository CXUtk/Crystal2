#pragma once
#include <Crystal2.h>
#include <memory>
#include <glm/glm.hpp>

class Material {
public:
    static std::shared_ptr<Material> CreateMaterial(JsonNode_CPTR pNode, const Scene* scene);

    Material() {}
    virtual ~Material() = 0 {};

    virtual std::shared_ptr<BSDF> ComputeScatteringFunctions(const SurfaceInteraction& isec, bool fromCamera = true) const = 0;
};

#pragma once
#include <Crystal2.h>
#include <memory>
#include <glm/glm.hpp>

class Material {
public:
    static std::shared_ptr<Material> CreateMaterial(const std::shared_ptr<SJson::SJsonNode>& node);

    Material() {}
    virtual ~Material() = 0 {};

    virtual std::shared_ptr<BSDF> ComputeScatteringFunctions(SurfaceInteraction& isec, bool fromCamera = true) const = 0;
};

#pragma once
#include <Crystal2.h>
#include <memory>

class Material {
public:
    Material() {}
    virtual ~Material() = 0 {};

    virtual std::shared_ptr<BSDF> ComputeScatteringFunctions(SurfaceInteraction& isec, bool fromCamera = true) const = 0;
};

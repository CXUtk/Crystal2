#pragma once
#include <Crystal2.h>
#include <memory>
#include <vector>

class Prototype {
public:
    Prototype(const std::shared_ptr<Light>& light, const std::shared_ptr<Material>& _material);
    ~Prototype();

    std::shared_ptr<Light> GetLight() const { return _light; }
    std::shared_ptr<Material> GetMaterial() const { return _material; }

    std::shared_ptr<BSDF> ComputeScatteringFunctions(SurfaceInteraction& isec, bool fromCamera = true) const;

private:
    std::shared_ptr<Light> _light;
    std::shared_ptr<Material> _material;
    // std::vector<std::shared_ptr<Texture>> _textures;
};
﻿#pragma once
#include <Crystal2.h>
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <Lights/AreaLight.h>

class Prototype {
public:
    static std::unique_ptr<Prototype> CreatePrototype(JsonNode_CPTR pNode, const Scene* scene);
    Prototype(const std::shared_ptr<AreaLight>& light, const std::shared_ptr<Material>& _material);
    ~Prototype();

    const AreaLight* GetAreaLight() const { return cptr(_light); }
    const Material* GetMaterial() const { return cptr(_material); }

    std::shared_ptr<BSDF> ComputeScatteringFunctions(SurfaceInteraction& isec, glm::vec3 dir, bool fromCamera = true) const;

private:
    std::shared_ptr<AreaLight> _light;
    std::shared_ptr<Material> _material;
    // std::vector<std::shared_ptr<Texture>> _textures;
};

#include "Prototype.h"
#include <Materials/Material.h>

Prototype::Prototype(const std::shared_ptr<Light>& light, const std::shared_ptr<Material>& material)
    : _light(light), _material(material) {
}

Prototype::~Prototype() {
}

std::shared_ptr<BSDF> Prototype::ComputeScatteringFunctions(SurfaceInteraction& isec, bool fromCamera) const {
    return _material->ComputeScatteringFunctions(isec, fromCamera);
}

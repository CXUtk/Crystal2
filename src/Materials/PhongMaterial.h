#pragma once
#include "Material.h"

namespace crystal
{
    class PhongMaterial : public Material
    {
    public:
        PhongMaterial(const Texture_RGB* Kd, int K);
        ~PhongMaterial() override;

        void ComputeScatteringFunctions(SurfaceInteraction& isec, 
            bool fromCamera = true) const override;

    private:
        const Texture_RGB* _Kd;
        int _K;
    };
}
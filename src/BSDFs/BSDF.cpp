#include "BSDF.h"
#include <glm/glm.hpp>
#include <Core/SurfaceInteraction.h>

BSDF::BSDF(const SurfaceInteraction* si) : _hit(si) {
}

void BSDF::AddBxDF(const std::shared_ptr<BxDF>& bxdf, float w) {
    _bxdfs.push_back(bxdf);
    _weights.push_back(w);
}

BxDFType BSDF::Flags() const {
    int sz = _bxdfs.size();
    int flag = 0;
    for (int i = 0; i < sz; i++) {
        flag |= _bxdfs[i]->GetType();
    }
    return (BxDFType)flag;
}

glm::vec3 BSDF::DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const {
    auto result = glm::vec3(0);
    bool reflect = glm::dot(_hit->GetNormal(), wIn) > 0;
    int sz = _bxdfs.size();
    float totalWeight = 0.f;
    for (int i = 0; i < sz; i++) {
        auto& bxdf = _bxdfs[i];
        if ((reflect && bxdf->Contains(BxDFType::BxDF_REFLECTION)) || (!reflect && bxdf->Contains(BxDFType::BxDF_TRANSMISSION))) {
            result += _weights[i] * bxdf->DistributionFunction(wOut, wIn);
            totalWeight += _weights[i];
        }
    }
    return result / totalWeight;
}

glm::vec3 BSDF::SampleDirection(float sampleBSDF, glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn, float* pdf, BxDFType sampleType) const {
    static float W[10];
    static int mapper[10];
    int sz = _bxdfs.size();
    int id = 0;
    // 按照概率分布均匀采样
    for (int i = 0; i < sz; i++) {
        auto& bxdf = _bxdfs[i];
        if (bxdf->Contains(sampleType)) {
            ++id;
            mapper[id] = i;
            W[id] = W[id - 1] + _weights[i];
        }
    }
    float v = sampleBSDF * W[id];
    std::shared_ptr<BxDF> bxdf = nullptr;
    for (int i = 0; i < id; i++) {
        if (W[i + 1] > v) {
            bxdf = _bxdfs[mapper[i + 1]];
            break;
        }
    }
    return bxdf->SampleDirection(sample, wOut, wIn, pdf);
}

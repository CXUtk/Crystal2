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
    int sz = _bxdfs.size();
    int tot = 0;
    int idMap[10];

    // 按照概率分布均匀采样
    for (int i = 0; i < sz; i++) {
        auto& bxdf = _bxdfs[i];
        if (bxdf->Contains(sampleType) && _weights[i] > 0.f) {
            idMap[tot] = i;
            ++tot;
        }
    }
    int v = (int)(std::min(0.99999994f, sampleBSDF) * tot);
    std::shared_ptr<BxDF> bxdf = _bxdfs[idMap[v]];
    auto L = bxdf->SampleDirection(sample, wOut, wIn, pdf);
    *pdf /= tot;

    return L;
}

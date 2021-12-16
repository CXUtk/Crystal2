#include "BSDF.h"
#include <glm/glm.hpp>
#include <Core/SurfaceInteraction.h>

BSDF::BSDF(const SurfaceInteraction* si) : _isec(si) {
}

void BSDF::AddBxDF(const std::shared_ptr<BxDF>& bxdf) {
    _bxdfs[_numBxDF++] = bxdf;
    assert(_numBxDF <= MAX_BxDFs);
}

BxDFType BSDF::Flags() const {
    int flag = 0;
    for (int i = 0; i < _numBxDF; i++) {
        flag |= _bxdfs[i]->GetType();
    }
    return (BxDFType)flag;
}

glm::vec3 BSDF::DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const {
    auto result = glm::vec3(0);
    bool reflect = glm::dot(_isec->GetNormal(), wIn) * glm::dot(_isec->GetNormal(), wOut) > 0;
    glm::vec3 L(0);
    for (int i = 0; i < _numBxDF; i++) {
        auto& bxdf = _bxdfs[i];
        if ((reflect && bxdf->Contains(BxDFType::BxDF_REFLECTION)) || (!reflect && bxdf->Contains(BxDFType::BxDF_TRANSMISSION))) {
            L += bxdf->DistributionFunction(_isec->GetInverseTNB() * wOut, _isec->GetInverseTNB() * wIn);
        }
    }
    return L;
}

glm::vec3 BSDF::SampleDirection(float sampleBSDF, glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn, 
    float* pdf, BxDFType flags, BxDFType* sampledType) const {
    assert(_numBxDF < 10);
    int tot = 0;
    int idMap[10] = {0};

    // 按照概率分布均匀采样
    for (int i = 0; i < _numBxDF; i++) {
        auto& bxdf = _bxdfs[i];
        if (bxdf->Contains(flags)) {
            idMap[tot] = i;
            ++tot;
        }
    }
    int v = (int)(std::min(0.9999999404f, sampleBSDF) * tot);
    std::shared_ptr<BxDF> bxdf = _bxdfs[idMap[v]];

    *sampledType = bxdf->GetType();

    wOut = _isec->GetInverseTNB() * wOut;
    auto L = bxdf->SampleDirection(sample, wOut, wIn, pdf, sampledType);
    *wIn = _isec->GetTNB() * (*wIn);

    *pdf /= tot;
    return L;
}

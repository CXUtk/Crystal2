#pragma once
#include <glm/glm.hpp>
#include <Crystal2.h>

class MicrofacetDistribution {
public:
    virtual ~MicrofacetDistribution() = 0 {};
    virtual float D(glm::vec3 wh) const = 0;
    virtual float Lambda(glm::vec3 w) const = 0;

    float G1(glm::vec3 w) const { return 1.f / (1.f + Lambda(w)); }
    float G(glm::vec3 wo, glm::vec3 wi) const { return 1.f / (1.f + Lambda(wo) + Lambda(wi)); }
};


class GGXDistribution : public MicrofacetDistribution {
public:
    GGXDistribution(const glm::mat3& TNB, float alphaX, float alphaY);
    ~GGXDistribution() override;

    float D(glm::vec3 wh) const override;
    float Lambda(glm::vec3 w) const override;

private:
    float _alphaX, _alphaY;
    glm::mat3 _TNB, _invTNB;
};

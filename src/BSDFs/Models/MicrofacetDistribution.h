#pragma once
#include <glm/glm.hpp>
#include <Crystal2.h>

class MicrofacetDistribution {
public:
    virtual ~MicrofacetDistribution() = 0 {};
    virtual float D(glm::vec3 wh) const = 0;
    virtual float G(glm::vec3 wo, glm::vec3 wi) const = 0;
    virtual glm::vec3 Sample_wh(glm::vec3 wo, glm::vec2 sample) const = 0;
    virtual float Pdf(glm::vec3 wi, glm::vec3 wh) const = 0;
};


class GGXDistribution : public MicrofacetDistribution {
public:
    GGXDistribution(const glm::mat3& TNB, float alphaX, float alphaY);
    ~GGXDistribution() override;

    float D(glm::vec3 wh) const override;
    float G(glm::vec3 wo, glm::vec3 wi) const override;
    glm::vec3 Sample_wh(glm::vec3 wo, glm::vec2 sample) const override;
    float Pdf(glm::vec3 wi, glm::vec3 wh) const override;

private:
    float _alphaX, _alphaY;
    glm::mat3 _TNB;

    float lambda(glm::vec3 w) const;
    float G1(glm::vec3 w) const;
};


class GGXRTDistribution : public MicrofacetDistribution {
public:
    GGXRTDistribution(const glm::mat3& TNB, float roughness);
    ~GGXRTDistribution() override;

    float D(glm::vec3 wh) const override;
    float G(glm::vec3 wo, glm::vec3 wi) const override;
    glm::vec3 Sample_wh(glm::vec3 wo, glm::vec2 sample) const override;
    float Pdf(glm::vec3 wi, glm::vec3 wh) const override;

private:
    float _roughness;
    glm::mat3 _TNB;

    float G1(glm::vec3 w) const;
};

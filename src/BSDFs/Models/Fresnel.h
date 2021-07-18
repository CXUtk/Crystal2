#pragma once
#include <glm/glm.hpp>
#include <Crystal2.h>

class Fresnel {
public:
    virtual ~Fresnel() = 0 {};
    virtual glm::vec3 Eval(float etaA, float etaB, float cosThetaI) const = 0;
};

class FresnelNoOp : public Fresnel {
public:
    FresnelNoOp() {}
    ~FresnelNoOp() override {}
    glm::vec3 Eval(float etaA, float etaB, float cosThetaI) const override { return glm::vec3(1.f); }
};


class FresnelDielectric : public Fresnel {
public:
    FresnelDielectric() {}
    ~FresnelDielectric() override {}
    glm::vec3 Eval(float etaA, float etaB, float cosThetaI) const override;
};


class FresnelSchlick : public Fresnel {
public:
    FresnelSchlick(glm::vec3 R0) :_r0(R0) {}
    ~FresnelSchlick() override {}
    glm::vec3 Eval(float etaA, float etaB, float cosThetaI) const override;

private:
    glm::vec3 _r0;
};

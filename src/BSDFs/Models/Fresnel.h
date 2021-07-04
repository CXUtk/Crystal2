#pragma once
#include <glm/glm.hpp>
#include <Crystal2.h>

class Fresnel {
public:
    virtual ~Fresnel() = 0 {};
    virtual float Eval(float cosThetaI) const = 0;
};

class FresnelNoOp: public Fresnel {
public:
    FresnelNoOp() {}
    ~FresnelNoOp() override {}
    float Eval(float cosThetaI) const override { return 1.f; }
};


class FresnelDielectric : public Fresnel {
public:
    FresnelDielectric(float etaA, float etaB) : _etaA(etaA), _etaB(etaB) {}
    ~FresnelDielectric() override {}
    float Eval(float cosThetaI) const override;

private:
    float _etaA, _etaB;
};

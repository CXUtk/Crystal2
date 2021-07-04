#pragma once
#include <glm/glm.hpp>
#include <Crystal2.h>

class Fresnel {
public:
    virtual ~Fresnel() = 0 {};
    virtual float Eval(float cosThetaI) const = 0;
};

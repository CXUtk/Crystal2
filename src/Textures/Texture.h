#pragma once
#include <Crystal2.h>

template <typename T>
class Texture {
public:
    virtual T Evaluate(const SurfaceInteraction&) const = 0;
    virtual ~Texture() = 0 {}
};

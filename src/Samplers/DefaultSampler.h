#pragma once
#include "Sampler.h"
#include <random>

class DefaultSampler : public Sampler {
public:
    DefaultSampler(int samplesPerPixel, int seed = 0);

    float Get1D() override;
    glm::vec2 Get2D() override;
    void Preprocess() override;
private:
    std::mt19937 mt;
};
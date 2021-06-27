#pragma once
#include "Sampler.h"
#include <random>

class DefaultSampler : public Sampler {
public:
    DefaultSampler(int samplesPerPixel, int seed = 0);

    float Get1D(int layer) override;
    glm::vec2 Get2D(int layer) override;
    void Preprocess() override;
private:
    std::mt19937 mt;
};
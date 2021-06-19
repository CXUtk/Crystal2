#pragma once
#include "Sampler.h"
#include <random>
#include <vector>

class StratifiedSampler : public Sampler {
public:
    StratifiedSampler(int samplesPerPixel, int seed = 0);
    ~StratifiedSampler();

    float Get1D() override;
    glm::vec2 Get2D() override;
    void Preprocess() override;
private:
    std::mt19937 mt;
    float* _sequence1D;
    glm::vec2* _sequence2D;

    float unifromFloat();
};

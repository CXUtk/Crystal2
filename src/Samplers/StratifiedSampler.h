#pragma once
#include "Sampler.h"
#include <random>
#include <vector>

class StratifiedSampler : public Sampler {
public:
    StratifiedSampler(int samplesPerPixel, int seed = 0);
    ~StratifiedSampler();

    void StartPixel(const glm::vec2& pt) override;
    float Get1D(int layer) override;
    glm::vec2 Get2D(int layer) override;
    void Preprocess() override;
    std::shared_ptr<Sampler> Clone(int seed) const override;

private:
    std::mt19937 mt;
    float* _sequence1D[2];
    glm::vec2* _sequence2D[2];
};

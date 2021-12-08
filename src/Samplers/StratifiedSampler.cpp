#include "StratifiedSampler.h"
#include <algorithm>

StratifiedSampler::StratifiedSampler(int samplesPerPixel, int seed) : Sampler(samplesPerPixel), mt(seed) {
    for (int i = 0; i < 2; i++) {
        _sequence1D[i] = new float[samplesPerPixel];
        _sequence2D[i] = new glm::vec2[samplesPerPixel];
    }
}

StratifiedSampler::~StratifiedSampler() {
    for (int i = 0; i < 2; i++) {
        delete[] _sequence1D[i];
        delete[] _sequence2D[i];
    }
}

void StratifiedSampler::StartPixel(const glm::vec2& pt) {
    Sampler::StartPixel(pt);
    Preprocess();
}

float StratifiedSampler::Get1D(int layer) {
    return _sequence1D[layer][_currentSampleIndex];
}

glm::vec2 StratifiedSampler::Get2D(int layer) {
    return _sequence2D[layer][_currentSampleIndex];
}

void StratifiedSampler::Preprocess() {
    int len = (int)std::sqrt(_samplesPerPixel);

    float d1d = 1.f / _samplesPerPixel;
    float d2d = 1.f / len;
    for (int l = 0; l < 2; l++) {
        for (int i = 0; i < _samplesPerPixel; i++) {
            _sequence1D[l][i] = i * d1d + uniformRandomFloat(mt) * d1d;
        }
        std::shuffle(_sequence1D[l], _sequence1D[l] + _samplesPerPixel, std::default_random_engine(mt()));

        for (int i = 0; i < len; i++) {
            for (int j = 0; j < len; j++) {
                _sequence2D[l][i * len + j] = glm::vec2(i, j) * d2d + glm::vec2(uniformRandomFloat(mt), uniformRandomFloat(mt)) * d2d;
            }
        }
        std::shuffle(_sequence2D[l], _sequence2D[l] + _samplesPerPixel, std::default_random_engine(mt()));
    }
}

std::shared_ptr<Sampler> StratifiedSampler::Clone(int seed) const {
    return std::make_shared<StratifiedSampler>(_samplesPerPixel, seed);
}

void StratifiedSampler::GenerateNextFrame(int sampleNum)
{}

float StratifiedSampler::GetFrame1D(glm::ivec2 coord)
{
    return 0.0f;
}

glm::vec2 StratifiedSampler::GetFrame2D(glm::ivec2 coord)
{
    return glm::vec2();
}


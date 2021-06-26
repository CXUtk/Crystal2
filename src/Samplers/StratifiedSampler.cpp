#include "StratifiedSampler.h"

StratifiedSampler::StratifiedSampler(int samplesPerPixel, int seed) : Sampler(samplesPerPixel), mt(seed) {
    _sequence1D = new float[samplesPerPixel];
    _sequence2D = new glm::vec2[samplesPerPixel];
}

StratifiedSampler::~StratifiedSampler() {
    delete[] _sequence1D;
    delete[] _sequence2D;
}

float StratifiedSampler::Get1D() {
    return _sequence1D[_currentSampleIndex];
}

glm::vec2 StratifiedSampler::Get2D() {
    return _sequence2D[_currentSampleIndex];
}

void StratifiedSampler::Preprocess() {
    int len = (int)std::sqrt(_samplesPerPixel);

    float d1d = 1.f / _samplesPerPixel;
    float d2d = 1.f / len;
    for (int i = 0; i < _samplesPerPixel; i++) {
        _sequence1D[i] = i * d1d + unifromFloat() * d1d;
    }

    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++) {
            _sequence2D[i * len + j] = glm::vec2(i, j) * d2d + glm::vec2(unifromFloat(), unifromFloat()) * d2d;
        }
    }
}

float StratifiedSampler::unifromFloat() {
    return (double)mt() / ((long long)mt.max() + 1LL);
}

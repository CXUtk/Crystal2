#include "DefaultSampler.h"

DefaultSampler::DefaultSampler(int samplesPerPixel, int seed) : Sampler(samplesPerPixel), mt(seed) {

}

float DefaultSampler::Get1D() {
    return (double)mt() / ((long long)mt.max() + 1LL);
}

glm::vec2 DefaultSampler::Get2D() {
    return glm::vec2((double)mt() / ((long long)mt.max() + 1LL), (double)mt() / ((long long)mt.max() + 1LL));
}

void DefaultSampler::Preprocess() {
}


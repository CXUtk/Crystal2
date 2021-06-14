#include "DefaultSampler.h"

DefaultSampler::DefaultSampler(int samplesPerPixel, int seed) : Sampler(samplesPerPixel), mt(seed) {

}

float DefaultSampler::Get1D() {
    return (float)mt() / mt.max();
}

glm::vec2 DefaultSampler::Get2D() {
    return glm::vec2((float)mt() / mt.max(), (float)mt() / mt.max());
}

void DefaultSampler::Preprocess() {
}


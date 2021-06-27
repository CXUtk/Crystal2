#include "DefaultSampler.h"

DefaultSampler::DefaultSampler(int samplesPerPixel, int seed) : Sampler(samplesPerPixel), mt(seed) {

}

float DefaultSampler::Get1D(int layer) {
    return uniformRandomFloat(mt);
}

glm::vec2 DefaultSampler::Get2D(int layer) {
    return glm::vec2(uniformRandomFloat(mt), uniformRandomFloat(mt));
}

void DefaultSampler::Preprocess() {
}


#pragma once
#include "Sampler.h"
#include <random>

class DefaultSampler : public Sampler
{
public:
	DefaultSampler(int samplesPerPixel, int seed = 0);

	float Get1D(int layer) override;
	glm::vec2 Get2D(int layer) override;
	void Preprocess() override;
	std::shared_ptr<Sampler> Clone(int seed) const override;

	void GenerateNextFrame(int sampleNum) override;
	float GetFrame1D(glm::ivec2 coord) override;
	glm::vec2 GetFrame2D(glm::ivec2 coord) override;

private:
	std::mt19937 mt;
};
#pragma once
#include "Sampler.h"
#include <random>
namespace crystal
{
	class DefaultSampler : public Sampler
	{
	public:
		DefaultSampler(int samplesPerPixel, int seed = 0);
		~DefaultSampler() override;

		float Get1D() override;

		Point2f Get2D() override;

		void Preprocess() override;

		std::shared_ptr<Sampler> Clone(int seed) const override;
	private:
		RNG _rng;
	};
}
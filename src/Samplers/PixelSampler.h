#pragma once
#include "Sampler.h"

namespace crystal
{
	class PixelSampler : public Sampler
	{
	public:
		PixelSampler(int samplesPerPixel, int dimensions);

		float Get1D() override;
		Point2f Get2D() override;
		bool StartNextSample() override;
		void StartPixel(const Point2i& pt) override;

		virtual std::shared_ptr<Sampler> Clone(int seed) const = 0;

	protected:
		std::vector<std::vector<float>> _samples1D;
		std::vector<std::vector<Point2f>> _samples2D;
		int _dimensions;
		int _current1DDimension = 0, _current2DDimension = 0;
		RNG _rng{};
	};
}
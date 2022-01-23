#pragma once
#include "Sampler.h"

namespace crystal
{
	class GlobalSampler : public Sampler
	{
	public:
		GlobalSampler(int samplesPerPixel);

		float Get1D() override;
		Point2f Get2D() override;
		bool StartNextSample() override;
		void StartPixel(const Point2i& pt) override;

		virtual std::shared_ptr<Sampler> Clone(int seed) const = 0;
		virtual int GetVectorIndexForSample(int sampleNum) const = 0;
		virtual float SampleValue(int vectorIndex, int dimension) const = 0;

	protected:
		int _currentDimension = 0;
		int _startVectorIndex = 0;
		static constexpr int ArrayStartDim = 5;
		int _arrayEndDim = 0;
	};
}
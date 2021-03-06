#pragma once
#include <Crystal2.h>
#include <Core/ConfigInfo.h>
#include <random>
#include <algorithm>

namespace crystal
{
	class Sampler
	{
	public:
		static std::shared_ptr<Sampler> LoadSampler(JsonNode_CPTR pSamplerNode,
			const config::ConfigInfo& configInfo);

		Sampler(int samplesPerPixel) : _samplesPerPixel(samplesPerPixel) {}
		int64_t GetSamplesPerPixel() const { return _samplesPerPixel; }

		virtual ~Sampler() = 0 {};
		virtual float Get1D() = 0;
		virtual Point2f Get2D() = 0;
		virtual std::shared_ptr<Sampler> Clone(int seed) const = 0;

		virtual void Request1DArray(int N);
		virtual void Request2DArray(int N);
		virtual bool StartNextSample();
		virtual void StartPixel(const Point2i& pt);

		float* Get1DArray(int N);
		Point2f* Get2DArray(int N);

	protected:
		int64_t _samplesPerPixel, _currentSampleIndex = 0;
		Point2i _currentPixel{};

		int _array1DOffset = 0, _array2DOffset = 0;

		std::vector<int> _samples1DArraySizes, _samples2DArraySizes;
		std::vector<std::vector<float>> _sampleArray1D;
		std::vector<std::vector<Point2f>> _sampleArray2D;
	};
}
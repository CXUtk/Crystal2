#include "PixelSampler.h"
#include <Core/Utils.h>

namespace crystal
{
	PixelSampler::PixelSampler(int samplesPerPixel, int dimensions) 
		: Sampler(samplesPerPixel), _dimensions(dimensions)
	{
		for (int i = 0; i < dimensions; i++)
		{
			_samples1D.push_back(std::vector<float>(samplesPerPixel));
			_samples2D.push_back(std::vector<Point2f>(samplesPerPixel));
		}
	}
	float PixelSampler::Get1D()
	{
		if (_current1DDimension < _samples1D.size())
		{
			return _samples1D[_current1DDimension++][_currentSampleIndex];
		}
		return uniformRandomFloat(_rng);
	}
	Point2f PixelSampler::Get2D()
	{
		if (_current2DDimension < _samples2D.size())
		{
			return _samples2D[_current2DDimension++][_currentSampleIndex];
		}
		return Point2f(uniformRandomFloat(_rng), uniformRandomFloat(_rng));
	}
	bool PixelSampler::StartNextSample()
	{
		_current1DDimension = 0, _current2DDimension = 0;
		return Sampler::StartNextSample();
	}
	void PixelSampler::StartPixel(const Point2i& pt)
	{
		_current1DDimension = 0, _current2DDimension = 0;
		Sampler::StartPixel(pt);
	}
}

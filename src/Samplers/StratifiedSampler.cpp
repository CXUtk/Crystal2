#include "StratifiedSampler.h"
#include <Core/Utils.h>
namespace crystal
{
	StratifiedSampler::StratifiedSampler(Point2i pixelSamples, int sampledDimensions)
		: PixelSampler(pixelSamples.x* pixelSamples.y, sampledDimensions), _pixelSamples(pixelSamples)
	{}

	void StratifiedSample1D(float* A, int size, RNG& rng)
	{
		float invNSamples = 1.f / size;
		for (int i = 0; i < size; i++)
		{
			A[i] = std::min(OneMinusEpsilon, invNSamples * (i + uniformRandomFloat(rng)));
		}
	}

	void StratifiedSample2D(Point2f* A, const Point2i& size, RNG& rng)
	{
		float invNSamplesX = 1.f / size.x;
		float invNSamplesY = 1.f / size.y;
		for (int i = 0; i < size.y; i++)
		{
			for (int j = 0; j < size.x; j++)
			{
				A->x = std::min(OneMinusEpsilon, invNSamplesX * (j + uniformRandomFloat(rng)));
				A->y = std::min(OneMinusEpsilon, invNSamplesY * (i + uniformRandomFloat(rng)));
				A++;
			}
		}
	}

	void StratifiedSampler::StartPixel(const Point2i& pt)
	{
		PixelSampler::StartPixel(pt);
		for (int i = 0; i < _samples1D.size(); i++)
		{
			StratifiedSample1D(&_samples1D[i][0], _pixelSamples.x * _pixelSamples.y, _rng);
			std::shuffle(_samples1D[i].begin(), _samples1D[i].begin() + _pixelSamples.x * _pixelSamples.y, _rng);
		}
		for (int i = 0; i < _samples2D.size(); i++)
		{
			StratifiedSample2D(&_samples2D[i][0], _pixelSamples, _rng);
			std::shuffle(_samples2D[i].begin(), _samples2D[i].begin() + _pixelSamples.x * _pixelSamples.y, _rng);
		}
	}
}
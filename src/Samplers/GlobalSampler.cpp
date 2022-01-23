#include "GlobalSampler.h"
#include <Core/Utils.h>

namespace crystal
{
	GlobalSampler::GlobalSampler(int samplesPerPixel)
		: Sampler(samplesPerPixel)
	{}

	float GlobalSampler::Get1D()
	{
		if (_currentDimension >= ArrayStartDim && _currentDimension < _arrayEndDim)
		{
			_currentDimension = _arrayEndDim;
		}
		return SampleValue(_startVectorIndex, _currentDimension++);
	}

	Point2f GlobalSampler::Get2D()
	{
		if (_currentDimension >= ArrayStartDim && _currentDimension < _arrayEndDim)
		{
			_currentDimension = _arrayEndDim;
		}
		auto sample = Point2f(SampleValue(_startVectorIndex, _currentDimension),
			SampleValue(_startVectorIndex, _currentDimension + 1));
		_currentDimension += 2;
		return sample;
	}

	bool GlobalSampler::StartNextSample()
	{
		_currentDimension = 0;
		_startVectorIndex = GetVectorIndexForSample(_currentSampleIndex + 1);
		return Sampler::StartNextSample();
	}

	void GlobalSampler::StartPixel(const Point2i& pt)
	{
		Sampler::StartPixel(pt);
		_currentDimension = 0;
		_startVectorIndex = GetVectorIndexForSample(0);
		_arrayEndDim = ArrayStartDim +
			_sampleArray1D.size() + 2 * _sampleArray2D.size();

		for (int i = 0; i < _samples1DArraySizes.size(); i++)
		{
			int count = _samples1DArraySizes[i] * _samplesPerPixel;
			for (int j = 0; j < count; j++)
			{
				int index = GetVectorIndexForSample(j);
				_sampleArray1D[i][j] = SampleValue(index, ArrayStartDim + i);
			}
		}

		int dim = ArrayStartDim + _samples1DArraySizes.size();
		for (int i = 0; i < _samples2DArraySizes.size(); i++)
		{
			int count = _samples2DArraySizes[i] * _samplesPerPixel;
			for (int j = 0; j < count; j++)
			{
				int index = GetVectorIndexForSample(j);
				_sampleArray2D[i][j].x = SampleValue(index, dim);
				_sampleArray2D[i][j].y = SampleValue(index, dim + 1);
			}
			dim += 2;
		}
		assert(dim == _arrayEndDim);
	}
}
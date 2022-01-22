#include "DefaultSampler.h"
#include <Core/Utils.h>
namespace crystal
{
	DefaultSampler::DefaultSampler(int samplesPerPixel, int seed) : Sampler(samplesPerPixel), _rng(seed)
	{

	}

	DefaultSampler::~DefaultSampler()
	{}

	float DefaultSampler::Get1D()
	{
		return uniformRandomFloat(_rng);
	}

	Point2f DefaultSampler::Get2D()
	{
		return Point2f(uniformRandomFloat(_rng), uniformRandomFloat(_rng));
	}


	std::shared_ptr<Sampler> DefaultSampler::Clone(int seed) const
	{
		auto other = std::make_shared<DefaultSampler>(*this);
		other->_rng.seed(seed);
		return other;
	}
}
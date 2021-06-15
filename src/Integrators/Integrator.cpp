#include "Integrator.h"
#include "WhittedIntegrator.h"
#include <Samplers/DefaultSampler.h>
std::shared_ptr<Integrator> Integrator::LoadIntegrator(const config::ConfigInfo& configInfo)
{
	std::shared_ptr<Sampler> sampler = nullptr;

	if (configInfo.SamplerType == "Default") {
		sampler = std::make_shared<DefaultSampler>(configInfo.SamplesPerPixel);
	}

	if (configInfo.IntegratorType == "WhittedStyle") {
		return std::make_shared<WhittedIntegrator>(sampler);
	}
}
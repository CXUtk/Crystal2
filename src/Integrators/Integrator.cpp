#include "Integrator.h"
#include "WhittedIntegrator.h"
#include <Samplers/DefaultSampler.h>
#include <SJson/SJson.h>

bool isSamplerIntegrator(const std::string& type) {
	return type == "WhittedStyle" || type == "PathTracer";
}

std::shared_ptr<Integrator> Integrator::LoadIntegrator(const std::shared_ptr<SJsonNode>& configNode, const config::ConfigInfo& configInfo) {
	assert(configNode->GetType() == SJsonNodeType::JSON_OBJECT);

	auto integratorNode = configNode->GetMember("Integrator");
	auto type = integratorNode->GetMember("Type")->GetString();
	if (isSamplerIntegrator(type)) {
		auto& samplerNode = integratorNode->GetMember("Sampler");
		auto sampler = Sampler::LoadSampler(samplerNode, config);

		if (type == "WhittedStyle") {
			return std::make_shared<WhittedIntegrator>(sampler);
		}
		else {
			throw std::invalid_argument("Invalid Integrator Type!");
		}
	}
	return nullptr;
}
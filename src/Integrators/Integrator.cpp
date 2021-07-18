#include "Integrator.h"
#include "PathTracingIntegrator.h"
#include <Samplers/DefaultSampler.h>
#include <SJson/SJson.h>

bool isSamplerIntegrator(const std::string& type) {
    return type == "WhittedStyle" || type == "PathTracer";
}

std::shared_ptr<Integrator> Integrator::LoadIntegrator(const std::shared_ptr<SJson::SJsonNode>& configNode, const config::ConfigInfo& configInfo) {
    assert(configNode->GetType() == SJson::SJsonNodeType::JSON_OBJECT);

    auto integratorNode = configNode->GetMember("Integrator");
    auto type = integratorNode->GetMember("Type")->GetString();
    if (isSamplerIntegrator(type)) {
        auto& samplerNode = integratorNode->GetMember("Sampler");
        auto sampler = Sampler::LoadSampler(samplerNode, configInfo);

        if (type == "PathTracer") {
            return std::make_shared<PathTracingIntegrator>(sampler, configInfo.NumOfThreads);
        }
        else {
            throw std::invalid_argument("Invalid Integrator Type!");
        }
    }
    else {
        throw std::invalid_argument("Invalid Integrator Type!");
    }
    return nullptr;
}

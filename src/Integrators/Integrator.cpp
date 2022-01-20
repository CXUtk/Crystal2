#include "Integrator.h"
#include "PathTracingIntegrator.h"
#include "DirectLightingIntegrator.h"
#include <Samplers/DefaultSampler.h>
#include <SJson/SJson.h>

namespace crystal
{
    bool isSamplerIntegrator(const std::string& type)
    {
        return true;
    }

    std::unique_ptr<Integrator> Integrator::LoadIntegrator(JsonNode_CPTR pConfigNode,
        const config::ConfigInfo& configInfo)
    {
        assert(pConfigNode->GetType() == SJson::SJsonNodeType::JSON_OBJECT);

        auto integratorNode = pConfigNode->GetMember("Integrator");
        auto type = integratorNode->GetMember("Type")->GetString();
        if (isSamplerIntegrator(type))
        {
            auto samplerNode = integratorNode->GetMember("Sampler");
            auto sampler = Sampler::LoadSampler(samplerNode, configInfo);

            if (type == "PathTracer")
            {
                auto maxDepth = integratorNode->GetMember("Depth")->GetInt();
                return std::make_unique<PathTracingIntegrator>(sampler, configInfo.NumOfThreads, maxDepth);
            }
            else if (type == "DirectLight")
            {
                auto maxDepth = integratorNode->GetMember("Depth")->GetInt();
                return std::make_unique<DirectLightingIntegrator>(sampler, configInfo.NumOfThreads, maxDepth);
            }
            else
            {
                throw std::invalid_argument("Invalid Integrator Type!");
            }
        }
        else
        {
            throw std::invalid_argument("Invalid Integrator Type!");
        }
        return nullptr;
    }
}
#include "Sampler.h"
#include <SJson/SJson.h>
#include "DefaultSampler.h"
#include "StratifiedSampler.h"

std::shared_ptr<Sampler> Sampler::LoadSampler(const std::shared_ptr<SJsonNode>& samplerNode, const config::ConfigInfo& configInfo) {
    assert(samplerNode->GetType() == SJsonNodeType::JSON_OBJECT);

    auto samplerType = samplerNode->GetMember("Type")->GetString();
    auto samplerSeed = samplerNode->GetMember("Seed")->GetInt();

    if (samplerType == "Default") {
        return std::make_shared<DefaultSampler>(configInfo.SamplesPerPixel, samplerSeed);
    }
    else if (samplerType == "Stratified") {
        return std::make_shared<StratifiedSampler>(configInfo.SamplesPerPixel, samplerSeed);
    }
    else {
        throw std::invalid_argument("Invalid Sampler Type!");
    }
    return nullptr;
}

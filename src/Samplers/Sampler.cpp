#include "Sampler.h"
#include <SJson/SJson.h>
#include "DefaultSampler.h"
#include "StratifiedSampler.h"

std::shared_ptr<Sampler> Sampler::LoadSampler(JsonNode_CPTR pSamplerNode, const config::ConfigInfo& configInfo) {
    assert(pSamplerNode->GetType() == SJson::SJsonNodeType::JSON_OBJECT);

    auto samplerType = pSamplerNode->GetMember("Type")->GetString();
    auto samplerSeed = pSamplerNode->GetMember("Seed")->GetInt();

    if (samplerType == "Default") {
        return std::make_shared<DefaultSampler>(configInfo.SamplesPerPixel, samplerSeed);
    }
    else if (samplerType == "Stratified") {
        bool isSQR = false;
        for (int i = 1; i <= configInfo.SamplesPerPixel; i++) {
            if ((long long)i * i > configInfo.SamplesPerPixel) {
                break;
            }
            if (i * i == configInfo.SamplesPerPixel) {
                isSQR = true;
            }
        }
        if (!isSQR) {
            throw std::exception("Stratified sampler should have a square number of SamplesPerPixel");
        }
        return std::make_shared<StratifiedSampler>(configInfo.SamplesPerPixel, samplerSeed);
    }
    else {
        throw std::invalid_argument("Invalid Sampler Type!");
    }
    return nullptr;
}

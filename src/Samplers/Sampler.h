#pragma once
#include <Crystal2.h>
#include <glm/glm.hpp>
#include <memory>
#include <Core/ConfigInfo.h>
class Sampler {
public:
    static std::shared_ptr<Sampler> LoadSampler(const std::shared_ptr<SJson::SJsonNode>& samplerNode, const config::ConfigInfo& configInfo);

    Sampler(int samplesPerPixel) : _samplesPerPixel(samplesPerPixel), _currentSampleIndex(0), _currentPixel(glm::vec2(0)){ }
    int GetSamplesPerPixel() const { return _samplesPerPixel; }

    virtual float Get1D() = 0;
    virtual glm::vec2 Get2D() = 0;
    virtual void StartPixel(const glm::vec2& pt) {
        _currentPixel = pt, _currentSampleIndex = 0;
    }
    virtual void Preprocess() = 0;
    virtual bool StartNextSample() {
        return ++_currentSampleIndex < _samplesPerPixel;
    }

protected:
    int _samplesPerPixel, _currentSampleIndex;
    glm::vec2 _currentPixel;
};
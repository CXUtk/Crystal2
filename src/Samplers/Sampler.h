#pragma once
#include <glm/glm.hpp>
class Sampler {
public:
    Sampler(int samplesPerPixel) : _samplesPerPixel(samplesPerPixel), _currentSampleIndex(0) { }
    int GetSamplesPerPixel() const { return _samplesPerPixel; }

    virtual float Get1D() = 0;
    virtual glm::vec2 Get2D() = 0;
    virtual void StartPixel(const glm::vec2& pt) { _currentPixel = pt; }
    virtual void Preprocess() = 0;
    virtual bool StartNextSample() {
        return ++_currentSampleIndex < _samplesPerPixel;
    }


protected:
    int _samplesPerPixel, _currentSampleIndex;
    glm::vec2 _currentPixel;
};
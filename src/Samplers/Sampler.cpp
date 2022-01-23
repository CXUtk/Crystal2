#include "Sampler.h"
#include <SJson/SJson.h>
#include "DefaultSampler.h"
#include "StratifiedSampler.h"
#include "HaltonSampler.h"

namespace crystal
{

    std::shared_ptr<Sampler> Sampler::LoadSampler(JsonNode_CPTR pSamplerNode, const config::ConfigInfo& configInfo)
    {
        assert(pSamplerNode->GetType() == SJson::SJsonNodeType::JSON_OBJECT);

        auto samplerType = pSamplerNode->GetMember("Type")->GetString();
        auto samplerSeed = pSamplerNode->GetMember("Seed")->GetInt();

        if (samplerType == "Default")
        {
            return std::make_shared<DefaultSampler>(configInfo.SamplesPerPixel, samplerSeed);
        }
        else if (samplerType == "Stratified")
        {
            bool isSQR = false;
            int side = 0;
            for (int i = 1; i <= configInfo.SamplesPerPixel; i++)
            {
                if ((long long)i * i > configInfo.SamplesPerPixel)
                {
                    break;
                }
                if (i * i == configInfo.SamplesPerPixel)
                {
                    side = i;
                    isSQR = true;
                }
            }
            if (!isSQR)
            {
                throw std::exception("Stratified sampler should have a square number of SamplesPerPixel");
            }
            return std::make_shared<StratifiedSampler>(Point2i(side, side), 18);
        }
        else if (samplerType == "Halton")
        {
            return std::make_shared<HaltonSampler>(configInfo.SamplesPerPixel, 
                Bound2i(Point2i(0, 0), Point2i(configInfo.Width, configInfo.Height)));
        }
        else
        {
            throw std::invalid_argument("Invalid Sampler Type!");
        }
        return nullptr;
    }

    void Sampler::Request1DArray(int N)
    {
        _samples1DArraySizes.push_back(N);
        _sampleArray1D.push_back(std::vector<float>(N * _samplesPerPixel));
    }

    void Sampler::Request2DArray(int N)
    {
        _samples2DArraySizes.push_back(N);
        _sampleArray2D.push_back(std::vector<Point2f>(N * _samplesPerPixel));
    }

    float* Sampler::Get1DArray(int N)
    {
        if (_array1DOffset == _sampleArray1D.size())
        {
            return nullptr;
        }
        return &_sampleArray1D[_array1DOffset++][_currentSampleIndex * N];
    }

    Point2f* Sampler::Get2DArray(int N)
    {
        if (_array2DOffset == _sampleArray2D.size())
        {
            return nullptr;
        }
        return &_sampleArray2D[_array2DOffset++][_currentSampleIndex * N];
    }

    bool Sampler::StartNextSample()
    {
        _array1DOffset = _array2DOffset = 0;
        return ++_currentSampleIndex < _samplesPerPixel;
    }

    void Sampler::StartPixel(const Point2i& pt)
    {
        _currentPixel = pt, _currentSampleIndex = 0;
        _array1DOffset = _array2DOffset = 0;
    }

}
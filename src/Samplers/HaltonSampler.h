#pragma once
#include "GlobalSampler.h"
#include <Core/Geometry.h>
#include <random>
#include <vector>

namespace crystal
{
    class HaltonSampler : public GlobalSampler
    {
    public:
        HaltonSampler(int samplesPerPixel, Bound2i sampleBound);
        ~HaltonSampler() override;


        std::shared_ptr<Sampler> Clone(int seed) const override;
        /**
         * @brief 获取第 sampleNum 次打到当前像素的采样向量的 index
         * @param sampleNum 
         * @return 
        */
        int GetVectorIndexForSample(int sampleNum) const override;
        float SampleValue(int vectorIndex, int dimension) const override;

    private:

        int _baseScales[2], _baseExponents[2];
        int _sampleStride;
        int _multInverse[2];
        mutable Point2i _oldPixel = Point2i(std::numeric_limits<int>::max(),
                                         std::numeric_limits<int>::max());
        mutable int64_t _offsetForCurrentPixel;
    };
}
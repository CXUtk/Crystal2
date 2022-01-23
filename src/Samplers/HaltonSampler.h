#pragma once
#include "GlobalSampler.h"
#include <random>
#include <vector>

namespace crystal
{
    class HaltonSampler : public GlobalSampler
    {
    public:
        HaltonSampler(int samplesPerPixel);
        ~HaltonSampler() override;

        virtual std::shared_ptr<Sampler> Clone(int seed) const override;

        void StartPixel(const Point2i& pt) override;

    private:
        Point2i _pixelSamples;
    };
}
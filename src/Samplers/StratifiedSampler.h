#pragma once
#include "PixelSampler.h"
#include <random>
#include <vector>

namespace crystal
{
    class StratifiedSampler : public PixelSampler
    {
    public:
        StratifiedSampler(Point2i pixelSamples, int sampledDimensions);
        ~StratifiedSampler();

        void StartPixel(const Point2i& pt) override;

    private:
        Point2i _pixelSamples;
    };
}
﻿#pragma once
#include "Accelerator.h"

class Brute : public Accelerator {
public:
    Brute();
    ~Brute() override;
    void Build(const std::vector<std::shared_ptr<Shape>>& objects) override;
    bool Intersect(const Ray& ray, SurfaceInteraction* info, float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const override;
    bool IntersectTest(const Ray& ray, float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const override;

private:
    std::vector<std::shared_ptr<Shape>> _objects;
};

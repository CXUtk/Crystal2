﻿#pragma once
#include <memory>
#include <string>
#include <vector>
#include <Crystal2.h>
#include <Core/Geometry.h>
#include <Core/SurfaceInteraction.h>
#include <Shapes/Shape.h>

class Accelerator {
public:
    Accelerator() {}
    virtual ~Accelerator() = 0 {}

    virtual void Build(const std::vector<std::shared_ptr<Shape>>& objects) = 0;
    virtual bool Intersect(const Ray& ray, SurfaceInteraction* info, float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const = 0;
    virtual bool IntersectTest(const Ray& ray, float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const = 0;

    static std::unique_ptr<Accelerator> GetAccelerator(const std::string& name);
};

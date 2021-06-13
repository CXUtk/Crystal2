#pragma once

#include <memory>
#include "Scene.h"
#include "FrameBuffer.h"

class RayTracer {
public:
    RayTracer();

    std::shared_ptr<FrameBuffer> Trace(const std::shared_ptr<Scene>& scene);

private:
    ~RayTracer();
};
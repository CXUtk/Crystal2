#pragma once
#include <Crystal2.h>
#include <memory>
#include <Loaders/ConfigLoader.h>
#include <thread>

class RayTracer {
public:
    RayTracer(const config::ConfigInfo& configInfo, JsonNode_CPTR pConfigNode,
        JsonNode_CPTR pSceneNode);
    ~RayTracer();

    void TraceAsync();
    const FrameBuffer* GetFrameBuffer() const { return ptr(_frameBuffer); }
    bool IsFinished() const { return _finished; }

private:
    std::unique_ptr<Scene> _scene;
    std::unique_ptr<Integrator> _integrator;
    std::unique_ptr<Camera> _camera;

    std::unique_ptr<FrameBuffer> _frameBuffer;
    std::unique_ptr<std::thread> _renderThread;

    int _width, _height;
    bool _finished;
};
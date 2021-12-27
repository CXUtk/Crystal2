#include "RayTracer.h"
#include <Integrators/Integrator.h>
#include <Core/FrameBuffer.h>
#include <Core/Scene.h>
#include <SJson/SJson.h>
#include <Cameras/Camera.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stbi/stb_image_write.h>

RayTracer::RayTracer(const config::ConfigInfo& configInfo, JsonNode_CPTR pConfigNode,
    JsonNode_CPTR pSceneNode) {
    _width = configInfo.Width;
    _height = configInfo.Height;

    _frameBuffer = std::make_unique<FrameBuffer>(_width, _height);

    _scene = Scene::CreateScene(pSceneNode->GetMember("Scene"), configInfo);
    _camera = Camera::CreateCamera(pSceneNode->GetMember("Camera"), configInfo);
    _integrator = Integrator::LoadIntegrator(pConfigNode, configInfo);

    _integrator->Preprocess(ptr(_scene));
}

void RayTracer::TraceAsync() {
    _frameBuffer->Clear();
    _finished = false;
    _renderThread = std::make_unique<std::thread>([&]() {

        clock_t startTime, endTime;
        startTime = clock();
        _integrator->Render(ptr(_scene), ptr(_camera), ptr(_frameBuffer));
        //------------------------------------------------
        endTime = clock();  //计时结束
        printf("The running time is: %.4fs\n",
            (double)(endTime - startTime) / CLOCKS_PER_SEC);

        _finished = true;
        auto& fb = _frameBuffer;
        stbi_write_hdr("result.hdr", fb->Width(), fb->Height(), 3, fb->GetImageDataRGB32F().get());
        fprintf(stdout, "Finished!\n");
    });
}

RayTracer::~RayTracer() {
    if(_renderThread)
        _renderThread->join();
}

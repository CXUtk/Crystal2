#include "RayTracer.h"
#include <Integrators/Integrator.h>
#include <Core/FrameBuffer.h>
#include <Core/Scene.h>
#include <SJson/SJson.h>
#include <Cameras/Camera.h>

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
    _integrator->Render(ptr(_scene), ptr(_camera), ptr(_frameBuffer));
}

RayTracer::~RayTracer() {

}

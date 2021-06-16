#include "RayTracer.h"
#include <Integrators/Integrator.h>
#include <Core/FrameBuffer.h>
#include <Core/Scene.h>
#include <SJson/SJson.h>
#include <Cameras/Camera.h>

RayTracer::RayTracer(const config::ConfigInfo& configInfo, const std::shared_ptr<SJsonNode>& configNode, const std::shared_ptr<SJsonNode>& sceneNode) {
    _width = configInfo.Width;
    _height = configInfo.Height;

    _scene = Scene::CreateScene(sceneNode->GetMember("Objects"));
    _camera = Camera::CreateCamera(sceneNode->GetMember("Camera"), configInfo);
    _integrator = Integrator::LoadIntegrator(configNode, configInfo);

    _integrator->Preprocess(_scene);
}

std::shared_ptr<FrameBuffer> RayTracer::Trace() {
    auto fb = std::make_shared<FrameBuffer>(_width, _height);
    fb->Clear();
    _integrator->Render(_scene, _camera, fb);
    return fb;
}

RayTracer::~RayTracer() {

}

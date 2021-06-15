#include "RayTracer.h"

RayTracer::RayTracer(const config::ConfigInfo& configInfo, const config::SceneInfo& sceneInfo) {
    _width = configInfo.Width;
    _height = configInfo.Height;

    _scene = Scene::CreateScene(sceneInfo);
    _camera = Camera::CreateCamera(sceneInfo);
    _integrator = Integrator::LoadIntegrator(configInfo);

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

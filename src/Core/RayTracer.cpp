#include "RayTracer.h"

RayTracer::RayTracer(const scene::SceneInfo& sceneInfo) {
    _width = sceneInfo.Width;
    _height = sceneInfo.Height;

    _scene = Scene::CreateScene(sceneInfo);
    _integrator = Integrator::LoadIntegrator(sceneInfo);
    _camera = Camera::CreateCamera(sceneInfo);

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

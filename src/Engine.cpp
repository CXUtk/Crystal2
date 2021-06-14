#include "Engine.h"
#include "Core/Scene.h"

Engine::Engine(const scene::SceneInfo& sceneInfo) {
    _width = sceneInfo.Width;
    _height = sceneInfo.Height;

    _rayTracer = std::make_shared<RayTracer>(sceneInfo);
}

Engine::~Engine() {
}

void Engine::Run() {
    auto fb = _rayTracer->Trace();
}

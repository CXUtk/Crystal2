#include "Engine.h"
#include <Core/RayTracer.h>
#include <SJson/SJson.h>
#include <Loaders/ConfigLoader.h>

Engine::Engine(const std::shared_ptr<SJsonNode>& configNode, const std::shared_ptr<SJsonNode>& sceneInfo) {
    _config = config::ConfigLoader::LoadConfigInfo(configNode);
    _rayTracer = std::make_shared<RayTracer>(_config, configNode, sceneInfo);
}

Engine::~Engine() {
}

void Engine::Run() {
    auto fb = _rayTracer->Trace();
}

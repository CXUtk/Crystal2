#include "Engine.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <Core/RayTracer.h>
#include <SJson/SJson.h>
#include <Loaders/ConfigLoader.h>
#include <stbi/stb_image_write.h>
#include <Core/FrameBuffer.h>

Engine::Engine(const std::shared_ptr<SJsonNode>& configNode, const std::shared_ptr<SJsonNode>& sceneInfo) {
    _config = config::ConfigLoader::LoadConfigInfo(configNode);
    _rayTracer = std::make_shared<RayTracer>(_config, configNode, sceneInfo);
}

Engine::~Engine() {
}

void Engine::Run() {
    auto fb = _rayTracer->Trace();
    stbi_write_png("result.png", fb->Width(), fb->Height(), 3, fb->GetImageData().get(), fb->Width() * 3);
    fprintf(stdout, "Finished!\n");
}

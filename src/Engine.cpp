#include "Engine.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <Core/RayTracer.h>
#include <SJson/SJson.h>
#include <Loaders/ConfigLoader.h>
#include <stbi/stb_image_write.h>
#include <Core/FrameBuffer.h>

Engine::Engine(const std::shared_ptr<SJson::SJsonNode>& configNode, const std::shared_ptr<SJson::SJsonNode>& sceneInfo) {
    try {
        _config = config::ConfigLoader::LoadConfigInfo(configNode);
        _rayTracer = std::make_shared<RayTracer>(_config, configNode, sceneInfo);
    }
    catch (const SJson::ConversionError& e) {
        fprintf(stderr, "%s: Cannot convert from %s to %s\n", e.what(), e.from.c_str(), e.to.c_str());
        throw;
    }
    catch (const SJson::MissingMemberError& e) {
        fprintf(stderr, "%s: Cannot find member named '%s'\n", e.what(), e.member.c_str());
        throw;
    }
    catch (const std::exception& e) {
        throw;
    }
}

Engine::~Engine() {
}

void Engine::Run() {
    fprintf(stdout, "All resources loaded, started ray tracing...\n");

    _rayTracer->TraceAsync();

    auto fb = _rayTracer->GetFrameBuffer();
    stbi_write_png("result.png", fb->Width(), fb->Height(), 3, fb->GetImageData().get(), fb->Width() * 3);
    fprintf(stdout, "Finished!\n");
}

#include "SamplerIntegrator.h"
#include <Core/FrameBuffer.h>
#include <Cameras/Camera.h>
#include <Samplers/DefaultSampler.h>

SamplerIntegrator::SamplerIntegrator(const std::shared_ptr<Sampler>& sampler) : Integrator(), _sampler(sampler) {
}


void SamplerIntegrator::Preprocess(const std::shared_ptr<Scene>& scene) {
    _sampler->Preprocess();
}

void SamplerIntegrator::Render(const std::shared_ptr<Scene>& scene,
    const std::shared_ptr<Camera>& camera,
    const std::shared_ptr<FrameBuffer>& frameBuffer) {
    int w = frameBuffer->Width(), h = frameBuffer->Height();

    size_t total = (size_t)w * h * _sampler->GetSamplesPerPixel();
    size_t current = 0;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            _sampler->StartPixel(glm::vec2(j, i));
            do {
                glm::vec2 pos = glm::vec2(j, i) + _sampler->Get2D();

                pos.x = pos.x / w;
                pos.y = pos.y / h;

                auto ray = camera->GenerateRay(pos);
                auto color = Evaluate(ray, scene, _sampler);

                frameBuffer->AddSample(j, i, color);
                current++;
            } while (_sampler->StartNextSample());
        }
        fprintf(stdout, "Tracing: %.2lf%%\n", (double)current / total * 100.0);
    }
}

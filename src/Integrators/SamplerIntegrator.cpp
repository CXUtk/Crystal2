#include "SamplerIntegrator.h"

SamplerIntegrator::SamplerIntegrator(const std::shared_ptr<Sampler>& sampler) : Integrator(), _sampler(sampler) {
}


void SamplerIntegrator::Preprocess(const std::shared_ptr<Scene>& scene) {
    _sampler->Preprocess();
}

void SamplerIntegrator::Render(const std::shared_ptr<Scene>& scene, 
    const std::shared_ptr<Camera>& camera,
    const std::shared_ptr<FrameBuffer>& frameBuffer) {
    int w = frameBuffer->Width(), h = frameBuffer->Height();
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            _sampler->StartPixel(glm::vec2(j, i));
            do {
                glm::vec2 pos = glm::vec2(j, i) + _sampler->Get2D();

                pos.x = pos.x / w;
                pos.y = pos.y / h;
                
                auto ray = camera->GenerateRay(pos);
                auto color = Evaluate(ray, scene);

                frameBuffer->AddSample(j, i, color);
            } while (_sampler->StartNextSample());
        }
    }
}

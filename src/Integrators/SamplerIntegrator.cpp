#include "SamplerIntegrator.h"
#include <Core/FrameBuffer.h>
#include <Cameras/Camera.h>
#include <Samplers/DefaultSampler.h>
#include <thread>
#include <mutex>


SamplerIntegrator::SamplerIntegrator(const std::shared_ptr<Sampler>& sampler, int threads)
    : Integrator(), _numThreads(threads)
{
    assert(threads <= 16);
    for (int i = 0; i < threads; i++)
    {
        _samplers[i] = sampler->Clone(i * 5 + 1);
    }
    _threadPool = std::make_unique<FixedThreadPool>(threads);
}

void SamplerIntegrator::Preprocess(Scene* scene)
{
    for (int i = 0; i < _numThreads; i++)
    {
        _samplers[i]->Preprocess();
    }
}

void SamplerIntegrator::Render(Scene* scene,
    Camera* camera,
    FrameBuffer* frameBuffer)
{
    int w = frameBuffer->Width(), h = frameBuffer->Height();

    size_t total = (size_t)w * h * _samplers[0]->GetSamplesPerPixel();
    size_t current = 0;

    std::mutex mutexLock;
    std::atomic<bool> canExit = false;

    for (int k = 0; k < _numThreads; k++)
    {
        for (int i = k; i < h; i += _numThreads)
        {
            _threadPool->RunAsync([&, i, k]() {
                for (int j = 0; j < w; j++)
                {
                    _samplers[k]->StartPixel(glm::vec2(j, i));
                    do
                    {
                        glm::vec2 pos = glm::vec2(j, i) + _samplers[k]->Get2D();
                        pos.x = pos.x / w;
                        pos.y = pos.y / h;

                        auto ray = camera->GenerateRay(pos);
                        auto color = Evaluate(ray, scene, ptr(_samplers[k]));

                        frameBuffer->AddSample(j, i, color);

                    } while (_samplers[k]->StartNextSample());
                }
                mutexLock.lock();
                current += w * _samplers[k]->GetSamplesPerPixel();
                fprintf(stdout, "Tracing: %.2lf%%\n", (double)current / total * 100.0);
                mutexLock.unlock();
                if (current == total)
                {
                    canExit = true;
                }
                });
        }
    }
    while (!canExit);
}
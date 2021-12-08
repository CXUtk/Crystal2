#include "SamplerIntegrator.h"
#include <Core/FrameBuffer.h>
#include <Cameras/Camera.h>
#include <Samplers/DefaultSampler.h>
#include <thread>
#include <mutex>


SamplerIntegrator::SamplerIntegrator(const std::shared_ptr<Sampler>& sampler, int threads)
    : Integrator(), _sampler(sampler), _numThreads(threads)
{
    assert(threads <= 16);
    _threadPool = std::make_unique<FixedThreadPool>(threads);
}

void SamplerIntegrator::Preprocess(Scene* scene)
{
    _sampler->Preprocess();
}

void SamplerIntegrator::Render(Scene* scene,
    Camera* camera,
    FrameBuffer* frameBuffer)
{
    int w = frameBuffer->Width(), h = frameBuffer->Height();
    int SPP = _sampler->GetSamplesPerPixel();

    size_t total = (size_t)w * h * SPP;

    size_t totalSampled = 0;

    std::mutex mutexLock;
    for (int k = 0; k < SPP; k++)
    {
        _sampler->GenerateNextFrame(k);
        int split = h % _numThreads;
        int block = h / _numThreads;

        std::atomic<bool> canExit = false;
        for (int mod = 0; mod < _numThreads; mod++)
        {
            size_t current = 0;
            size_t totalSamplesThisTask = (size_t)w * ((mod < split) ? (block + 1) : (block));

            _threadPool->RunAsync([&, mod]() {
                for (int i = mod; i < h; i += _numThreads)
                {
                    for (int j = 0; j < w; j++)
                    {
                        glm::vec2 pos = glm::vec2(j, i) + _sampler->GetFrame2D(glm::ivec2(j, i));
                        pos.x = pos.x / w;
                        pos.y = pos.y / h;

                        auto ray = camera->GenerateRay(pos);
                        auto color = Evaluate(ray, scene, ptr(_sampler));

                        frameBuffer->AddSample(j, i, color);
                    }
                }

                mutexLock.lock();
                current += totalSamplesThisTask;
                totalSampled += totalSamplesThisTask;
                fprintf(stdout, "Tracing: %.2lf%%\n", (double)totalSampled / total * 100.0);
                mutexLock.unlock();

                if (current == w * h)
                {
                    canExit = true;
                }
            });
        }
        // Wait until finish a frame
        while (!canExit) {}
    }

}
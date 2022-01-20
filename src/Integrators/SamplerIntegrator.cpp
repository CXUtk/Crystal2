#include "SamplerIntegrator.h"
#include <Core/FrameBuffer.h>
#include <Cameras/Camera.h>
#include <Samplers/DefaultSampler.h>
#include <thread>
#include <mutex>
namespace crystal
{

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

        //int split = h % _numThreads;
        //int blockLower = h / _numThreads;
        //int blockUpper = (h + _numThreads - 1) / _numThreads;
        std::atomic<bool> canExit = false;
        for (int mod = 0; mod < _numThreads; mod++)
        {
            auto task = [&, mod, w, h]() {
                auto sampler_thread = _sampler->Clone(0);
                for (int i = mod; i < h; i += _numThreads)
                {
                    for (int j = 0; j < w; j++)
                    {
                        sampler_thread->StartPixel(Point2i(i, j));
                        do
                        {
                            glm::vec2 pos = glm::vec2(j, i) + sampler_thread->Get2D();
                            pos.x = pos.x / w;
                            pos.y = pos.y / h;

                            auto ray = camera->GenerateRay(pos);
                            auto color = Evaluate(ray, scene, ptr(_sampler));

                            frameBuffer->AddSample(j, i, color);
                        } while (sampler_thread->StartNextSample());

                        mutexLock.lock();
                        totalSampled += SPP;
                        fprintf(stdout, "Tracing: %.2lf%%\n", (double)totalSampled / total * 100.0);
                        mutexLock.unlock();
                    }
                }

                //size_t totalSamplesThisTask = (size_t)w * ((mod < split) ? (blockUpper) : (blockLower));
                //for (int k = 0; k < SPP; k++)
                //{
                //    for (int i = mod; i < h; i += _numThreads)
                //    {
                //        for (int j = 0; j < w; j++)
                //        {
                //            glm::vec2 pos = glm::vec2(j, i) + _sampler->GetFrame2D(glm::ivec2(j, i));
                //            pos.x = pos.x / w;
                //            pos.y = pos.y / h;

                //            auto ray = camera->GenerateRay(pos);
                //            auto color = Evaluate(ray, scene, ptr(_sampler));

                //            frameBuffer->AddSample(j, i, color);
                //        }
                //    }
                // }
                if (totalSampled == total)
                {
                    canExit = true;
                }
            };
            if (mod < _numThreads - 1)
            {
                _threadPool->RunAsync(task);
            }
            else
            {
                task();
            }
        }
        // Wait until finish a frame
        while (!canExit) {}
    }
}
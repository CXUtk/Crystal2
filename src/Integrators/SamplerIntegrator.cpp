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

    void SamplerIntegrator::Render(Scene* scene,
        Camera* camera,
        FrameBuffer* frameBuffer)
    {
        Preprocess(scene, ptr(_sampler));
        int w = frameBuffer->Width(), h = frameBuffer->Height();
        int SPP = _sampler->GetSamplesPerPixel();

        constexpr int TILE_SIZE = 32;
        Point2i nTiles((w + TILE_SIZE - 1) / TILE_SIZE, (h + TILE_SIZE - 1) / TILE_SIZE);

        std::atomic<int> finishedCount = 0;

        for (int x = 0; x < nTiles.x; x++)
        {
            for (int y = 0; y < nTiles.y; y++)
            {
                auto task = [&, x, y]() {
                    int x0 = TILE_SIZE * x;
                    int x1 = std::min(x0 + TILE_SIZE, w);
                    int y0 = TILE_SIZE * y;
                    int y1 = std::min(y0 + TILE_SIZE, h);

                    auto sampler_thread = _sampler->Clone(x * nTiles.y + y);

                    for (int xx = x0; xx < x1; xx++)
                    {
                        for (int yy = y0; yy < y1; yy++)
                        {
                            sampler_thread->StartPixel(Point2i(xx, yy));
                            do
                            {
                                auto s = sampler_thread->Get2D();
                                glm::vec2 pos = glm::vec2(xx, yy) + s;
                                pos.x = pos.x / w;
                                pos.y = pos.y / h;

                                auto ray = camera->GenerateRay(pos);
                                auto color = Evaluate(ray, scene, ptr(sampler_thread));

                                frameBuffer->AddSample(xx, yy, color);
                            } while (sampler_thread->StartNextSample());
                        }
                    }
                    
                    finishedCount++;
                };
                _threadPool->RunAsync(task);
            }
        }
        // Wait until finish a frame
        while (finishedCount < nTiles.x * nTiles.y) {}
    }
}
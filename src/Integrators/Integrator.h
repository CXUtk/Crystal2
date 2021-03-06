#pragma once
#include <Crystal2.h>
#include <memory>
#include <Core/ConfigInfo.h>

namespace crystal
{
    class Integrator
    {
    public:
        static std::unique_ptr<Integrator> LoadIntegrator(JsonNode_CPTR pConfigNode,
            const config::ConfigInfo& configInfo);

        Integrator() {}
        virtual ~Integrator() = 0 {}
        virtual void Render(Scene* scene, Camera* camera,
            FrameBuffer* frameBuffer) = 0;
    };
}
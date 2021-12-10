#pragma once
#include <Crystal2.h>
#include <glm/glm.hpp>

namespace crystal
{
    enum LightFlags : int
    {
        DeltaPosition = 1 << 0, 
        DeltaDirection = 1 << 1,
        Area = 1 << 2, 
        Infinite = 1 << 3
    };

    class Light
    {
    public:
        static std::shared_ptr<Light> CreateLight(JsonNode_CPTR pNode, const Scene* scene);


        Light(LightFlags flags, int numSamples) : _flags(flags), _numSamples(numSamples) { }


        virtual ~Light() = 0 {}

        virtual glm::vec3 Flux() const = 0;

        virtual void Preprocess(const Scene* scene) const { }

        // Get the radiance value from the light to the object surface hit point
        virtual glm::vec3 Sample_Li(const SurfaceInteraction& hit, const glm::vec2& sample, glm::vec3* endpoint, float* pdf) const = 0;

        virtual glm::vec3 Sample_Le(const SurfaceInteraction& hit, const glm::vec2& sample, glm::vec3* endpoint, float* pdf) const = 0;

        int GetNumSamples() const { return _numSamples; }
        LightFlags GetFlags() const { return _flags; }

    private:
        int _numSamples;
        LightFlags _flags;
    };
}

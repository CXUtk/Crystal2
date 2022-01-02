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
        static std::shared_ptr<AreaLight> CreateAreaLight(JsonNode_CPTR pNode, const Shape* shape, 
            const Scene* scene);

        Light(LightFlags flags, int numSamples) : _flags(flags), _numSamples(numSamples) { }

        virtual ~Light() = 0 {}

        virtual const crystal::IIntersectable* GetAttachedObject() const { return nullptr; }

        virtual Spectrum Flux() const = 0;

        virtual void Preprocess(const Scene* scene) const { }

        // Get the radiance value from the light to the object surface hit point
        virtual Spectrum Sample_Li(const SurfaceInfo& surface_w, const glm::vec2& sample,
            Point3f* endpoint, float* pdf) const = 0;

        virtual float Pdf_Li(const SurfaceInfo& surface_w, const Vector3f& wi) const = 0;

        // Infinite area light, get emitted radiance from direction
        virtual Spectrum Le(const Vector3f& wi) const { return Spectrum(0.f); }

        int GetNumSamples() const { return _numSamples; }

        LightFlags GetFlags() const { return _flags; }

        bool IsDeltaLight() const
        {
            return (_flags & DeltaPosition) || (_flags & DeltaDirection);
        }

    private:
        int _numSamples;
        LightFlags _flags;
    };
}

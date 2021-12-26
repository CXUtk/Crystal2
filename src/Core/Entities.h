#pragma once
#include <Crystal2.h>
#include "Geometry.h"

namespace crystal
{
	class IIntersectable
	{
	public:
		virtual ~IIntersectable() = 0 {}
		virtual BoundingBox GetBoundingBox() const = 0;
		virtual bool Intersect(const Ray& ray, SurfaceInteraction* info) const = 0;
		virtual bool IntersectTest(const Ray& ray, float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const = 0;
	};

	class Entity : public IIntersectable
	{
	public:
		virtual ~Entity() = 0 {}
		virtual const Material* GetMaterial() const = 0;
		virtual const AreaLight* GetAreaLight() const = 0;

		virtual void ComputeScatteringFunctions(SurfaceInteraction& isec, bool fromCamera = true) const = 0;
	private:
	};


	class GeometricEntity : public Entity
	{
	public:
		GeometricEntity(const Shape* shape, const Material* material, const AreaLight* areaLight);
		~GeometricEntity() override;

		BoundingBox GetBoundingBox() const override;
		bool Intersect(const Ray& ray, SurfaceInteraction* info) const override;
		bool IntersectTest(const Ray& ray, float tMin = 0, float tMax = std::numeric_limits<float>::infinity()) const override;

		const Material* GetMaterial() const override { return _material; }
		const AreaLight* GetAreaLight() const override { return _areaLight; }

		virtual void ComputeScatteringFunctions(SurfaceInteraction& isec, bool fromCamera = true) const override;
	private:
		const Shape* _shape;
		const Material* _material;
		const AreaLight* _areaLight;
	};
}
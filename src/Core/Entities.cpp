#include "Entities.h"
#include <Shapes/Shape.h>
#include <Materials/Material.h>

crystal::GeometricEntity::GeometricEntity(const Shape* shape, const Material* material, const AreaLight* areaLight)
	: _shape(shape), _material(material), _areaLight(areaLight)
{

}

crystal::GeometricEntity::~GeometricEntity()
{
	
}

BoundingBox crystal::GeometricEntity::GetBoundingBox() const
{
	return BoundingBox();
}

bool crystal::GeometricEntity::Intersect(const Ray& ray, SurfaceInteraction* info) const
{
	return _shape->Intersect(ray, info);
}

bool crystal::GeometricEntity::IntersectTest(const Ray& ray, float tMin, float tMax) const
{
	return _shape->IntersectTest(ray, tMin, tMax);
}

void crystal::GeometricEntity::ComputeScatteringFunctions(SurfaceInteraction& isec, bool fromCamera) const
{
	_material->ComputeScatteringFunctions(isec, fromCamera);
}

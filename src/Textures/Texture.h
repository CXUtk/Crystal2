#pragma once
#include <Crystal2.h>

template <typename T>
class Texture
{
public:
	virtual T Evaluate(const SurfaceInteraction&) const = 0;
	virtual ~Texture() = 0 {}
};



class PureTexture : public Texture<glm::vec3>
{
public:
	PureTexture(glm::vec3 color) : _color(color) {}
	glm::vec3 Evaluate(const SurfaceInteraction&) const override { return _color; }
	~PureTexture() override {}

private:
	glm::vec3 _color;
};
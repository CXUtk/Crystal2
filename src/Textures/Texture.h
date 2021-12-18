#pragma once
#include <Crystal2.h>

template <typename T>
class Texture
{
public:
	virtual T Evaluate(const SurfaceInteraction&) const = 0;
	virtual ~Texture() = 0 {}
};


template <typename T>
class PureTexture : public Texture<T>
{
public:
	PureTexture(T value) : _value(value) {}
	T Evaluate(const SurfaceInteraction&) const override { return _value; }
	~PureTexture() override {}

private:
	T _value;
};
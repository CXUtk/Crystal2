﻿#pragma once
#include <glm/glm.hpp>
#include <Crystal2.h>

class MicrofacetDistribution
{
public:
	virtual ~MicrofacetDistribution() = 0 {};
	virtual float D(const Vector3f& wh) const = 0;
	virtual float G(const Vector3f& wo, const Vector3f& wi) const = 0;
	virtual Vector3f Sample_wh(const Vector3f& wo, glm::vec2 sample) const = 0;
	virtual float Pdf(const Vector3f& wo, const Vector3f& wh) const = 0;
};


class GGXDistribution : public MicrofacetDistribution
{
public:
	GGXDistribution(Vector2f roughness);
	~GGXDistribution() override;

	float D(const Vector3f& wh) const override;
	float G(const Vector3f& wo, const Vector3f& wi) const override;
	Vector3f Sample_wh(const Vector3f& wo, glm::vec2 sample) const override;
	float Pdf(const Vector3f& wo, const Vector3f& wh) const override;

private:
	Vector2f _alpha;

	float lambda(const Vector3f& wh) const;
};


class BeckmannDistribution : public MicrofacetDistribution
{
public:
	// BeckmannDistribution Public Methods
	static float RoughnessToAlpha(float roughness);


	BeckmannDistribution(Vector2f roughness);
	~BeckmannDistribution() override;

	float D(const Vector3f& wh) const override;
	float G(const Vector3f& wo, const Vector3f& wi) const override;
	Vector3f Sample_wh(const Vector3f& wo, glm::vec2 sample) const override;
	float Pdf(const Vector3f& wo, const Vector3f& wh) const override;

private:
	Vector2f _alpha;

	float lambda(const Vector3f& wh) const;
};
#pragma once
#include <glm/glm.hpp>
#include <Crystal2.h>

class MicrofacetDistribution
{
public:
	virtual ~MicrofacetDistribution() = 0 {};
	virtual float D(const Vector3f& wh) const = 0;
	virtual float G(const Vector3f& wo, const Vector3f& wi) const = 0;
	virtual Vector3f Sample_wh(const Vector3f& wo, glm::vec2 sample) const = 0;
	virtual float Pdf(const Vector3f& wi, const Vector3f& wh) const = 0;
};


class GGXDistribution : public MicrofacetDistribution
{
public:
	GGXDistribution(const glm::mat3& TNB, float alphaX, float alphaY);
	~GGXDistribution() override;

	float D(const Vector3f& wh) const override;
	float G(const Vector3f& wo, const Vector3f& wi) const override;
	glm::vec3 Sample_wh(const Vector3f& wo, glm::vec2 sample) const override;
	float Pdf(const Vector3f& wi, const Vector3f& wh) const override;

private:
	float _alphaX, _alphaY;
	glm::mat3 _TNB;

	float lambda(glm::vec3 w) const;
	float G1(glm::vec3 w) const;
};


class GGXRTDistribution : public MicrofacetDistribution
{
public:
	GGXRTDistribution(const glm::mat3& TNB, float roughness);
	~GGXRTDistribution() override;

	float D(const Vector3f& wh) const override;
	float G(const Vector3f& wo, const Vector3f& wi) const override;
	glm::vec3 Sample_wh(const Vector3f& wo, glm::vec2 sample) const override;
	float Pdf(const Vector3f& wi, const Vector3f& wh) const override;

private:
	float _roughness;
	glm::mat3 _TNB;

	float G1(glm::vec3 w) const;
};


class BeckmannDistribution : public MicrofacetDistribution
{
public:
	BeckmannDistribution(float alphaX, float alphaY);
	~BeckmannDistribution() override;

	float D(const Vector3f& wh) const override;
	float G(const Vector3f& wo, const Vector3f& wi) const override;
	Vector3f Sample_wh(const Vector3f& wo, glm::vec2 sample) const override;
	float Pdf(const Vector3f& wi, const Vector3f& wh) const override;

private:
	Vector2f _alpha;

	float lambda(const Vector3f& wh) const;
	float G1(const Vector3f& w) const;
};
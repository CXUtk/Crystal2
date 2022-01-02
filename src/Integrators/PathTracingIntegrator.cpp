﻿#include "PathTracingIntegrator.h"

#include <Core/Scene.h>
#include <Core/Geometry.h>
#include <Core/SurfaceInteraction.h>
#include <Core/Entities.h>

#include <glm/gtx/transform.hpp>
#include <BSDFs/BSDF.h>
#include <Samplers/DefaultSampler.h>
#include <Lights/Light.h>
#include <Lights/AreaLight.h>

#include <Textures/CubemapTexture.h>
#include <Core/Utils.h>

static constexpr float pRR = 0.8f;

PathTracingIntegrator::PathTracingIntegrator(const std::shared_ptr<Sampler>& sampler, int threads, int maxDepth)
    : SamplerIntegrator(sampler, threads), _maxDepth(maxDepth) {

}


// 路径追踪渲染器一开始使用选定sampler，在经过一次transport以后变成随机sampler
glm::vec3 PathTracingIntegrator::Evaluate(const Ray& ray, Scene* scene,
    Sampler* sampler) {
    return eval_rec(ray, scene, sampler, 0, true);
}


glm::vec3 PathTracingIntegrator::eval_rec(const Ray& ray, Scene* scene,
	Sampler* sampler, int level, bool specular)
{
	Spectrum L(0.f);
	if (level == _maxDepth) return L;

	SurfaceInteraction isec;
	if (!scene->Intersect(ray, &isec))
	{
		if (scene->GetSkybox() == nullptr) return glm::vec3(0.f);
		return scene->GetSkybox()->Evaluate(ray.dir);
	}

	Normal3f N = isec.GetNormal();
	Point3f P = isec.GetHitPos();
	auto E = isec.GetHitEntity();

	// 如果是自发光物体就把发光项加上
	if (specular)
	{
		L += isec.Le(-ray.dir);
	}

	BSDF bsdf(&isec);
	isec.SetBSDF(&bsdf);
	isec.GetHitEntity()->ComputeScatteringFunctions(isec, true);

	// No bsdf function, means the object is transparent
	if (bsdf.IsEmpty())
	{
		if (isec.GetHitEntity()->GetAreaLight() != nullptr)
		{
			return L;
		}
		return eval_rec(isec.SpawnRay(ray.dir), scene, sampler, level, specular);
	}


	// 进行一次路径追踪采样
	Vector3f wIn;
	float pdf;
	BxDFType type;
	auto brdf = bsdf.SampleDirection(sampler->Get1D(), sampler->Get2D(), -ray.dir, &wIn,
		&pdf, BxDFType::BxDF_ALL, &type);
	NAN_DETECT_V(brdf, "PathTracingIntegrator::BSDF");
	INF_DETECT_V(brdf, "PathTracingIntegrator::BSDF");
	if (std::abs(pdf) == 0.f || brdf == glm::vec3(0)) return L;
	bool isSpecular = (type & BxDF_SPECULAR);

	auto cosine = isSpecular ? 1.0f : std::max(0.f, (type & BxDF_TRANSMISSION)
	   ? glm::dot(-N, wIn) : glm::dot(N, wIn));
	auto Lindir = eval_rec(isec.SpawnRay(wIn), scene, sampler, level + 1, isSpecular) * brdf * cosine / pdf;

	// 计算从光源采样的radiance
	L += UniformSampleAllLights(isec, scene, sampler);

	L += Lindir;
	NAN_DETECT_V(L, "PathTracingIntegrator::L");
	INF_DETECT_V(L, "PathTracingIntegrator::L");
	return L;
}


Spectrum PathTracingIntegrator::UniformSampleAllLights(const SurfaceInteraction& isec, Scene* scene, Sampler* sampler)
{
	Spectrum L(0.f);
	for (auto& light : scene->GetLights())
	{
		if (light->Flux() == Spectrum(0.f)) continue;
		// One sample for each light
		glm::vec2 sampleLight = sampler->Get2D();
		glm::vec2 sampleBSDF = sampler->Get2D();

		L += EsimateDirect(isec, scene, sampleLight, sampleBSDF, light, sampler);
	}
	return L;
}

Spectrum PathTracingIntegrator::EsimateDirect(const SurfaceInteraction& isec, Scene* scene,
	glm::vec2 sampleLight, glm::vec2 sampleBSDF, const crystal::Light* light, Sampler* sampler)
{
	Spectrum L(0.f);

	BxDFType sampleType = (BxDFType)(BxDFType::BxDF_ALL & ~BxDFType::BxDF_SPECULAR);
	Point3f P = isec.GetHitPos();
	Normal3f N = isec.GetNormal();
	Vector3f wo = -isec.GetHitDir();

	// Sample light source with MIS (Specular BSDF will not have value)
	Point3f lightPos;
	float pdf_light;
	auto Li_light = light->Sample_Li(isec.GetSurfaceInfo(false), sampleLight, &lightPos, &pdf_light);

	Vector3f wi = glm::normalize(lightPos - P);
	if (pdf_light != 0.f && Li_light != Spectrum(0.f))
	{
		Spectrum f = isec.GetBSDF()->DistributionFunction(wo, wi)
			* std::max(0.f, glm::dot(N, wi));
		float pdf_bsdf = isec.GetBSDF()->Pdf(wo, wi, sampleType);
		if (f != Spectrum(0.f))
		{
			if (scene->IntersectTest(isec.SpawnRayTo(lightPos), 0, 1 - EPS, 
				light->GetAttachedObject()))
			{
				Li_light = Spectrum(0.f);
			}

			if (Li_light != Spectrum(0.f))
			{
				if (light->IsDeltaLight())
				{
					L += f * Li_light / pdf_light;
				}
				else
				{
					L += f * Li_light * PowerHeuristic(1, pdf_light, 1, pdf_bsdf) / pdf_light;
				}
			}
		}
	}

	// Sample BSDF (Delta light should not have any value in BSDF sample)
	if (!light->IsDeltaLight())
	{
		float pdf_bsdf;
		BxDFType sampledType;
		Spectrum f = isec.GetBSDF()->SampleDirection(sampler->Get1D(), sampleBSDF, wo, &wi, &pdf_bsdf,
			sampleType, &sampledType);
		bool specularBSDF = sampledType & BxDF_SPECULAR;

		float weight = 1.0f;
		if (!specularBSDF)
		{
			f *= std::max(0.f, glm::dot(N, wi));
			pdf_light = light->Pdf_Li(isec.GetSurfaceInfo(false), wi);
			if (pdf_light == 0.f) return L;
			weight = PowerHeuristic(1, pdf_bsdf, 1, pdf_light);
		}

		SurfaceInteraction lightIsec;
		Spectrum Li(0.f);
		if (scene->Intersect(isec.SpawnRay(wi), &lightIsec))
		{
			if (lightIsec.GetHitEntity()->GetAreaLight() == light)
			{
				Li = lightIsec.Le(-wi);
			}
		}
		else
		{
			Li = light->Le(wi);
		}
		if (Li != Spectrum(0.f))
		{
			L += f * Li * weight / pdf_bsdf;
		}
	}
	return L;
}

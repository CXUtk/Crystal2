#include "PathTracingIntegrator.h"

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


glm::vec3 PathTracingIntegrator::Evaluate(const Ray& ray, Scene* scene,
	Sampler* sampler)
{
	int bounces = 0;
	bool lightPath = true;
	Spectrum beta(1.f), L(0.f);
	Ray currentRay(ray);

	for (; bounces < _maxDepth; bounces++)
	{
		SurfaceInteraction isec;
		if (!scene->Intersect(currentRay, &isec))
		{
			if (scene->GetSkybox() != nullptr)
			{
				L += beta * scene->GetSkybox()->Evaluate(currentRay.dir);
			}
			else
			{
				L += beta * 0.5f;
			}
			break;
		}

		Vector3f wo = -currentRay.dir;
		Normal3f N = isec.GetNormal();
		Point3f P = isec.GetHitPos();
		auto E = isec.GetHitEntity();

		// 如果是自发光物体就把发光项加上
		if (lightPath)
		{
			L += beta * isec.Le(-currentRay.dir);
		}

		BSDF bsdf(&isec);
		isec.SetBSDF(&bsdf);
		isec.GetHitEntity()->ComputeScatteringFunctions(isec, true);

		// No bsdf function, means the object is transparent, go through it
		if (bsdf.IsEmpty())
		{
			currentRay = isec.SpawnRay(currentRay.dir);
			continue;
		}

		// 计算从光源采样的radiance
		L += beta * UniformSampleAllLights(isec, scene, sampler);
		NAN_DETECT_V(L, "PathTracingIntegrator::L::UniformSampleAllLights");
		INF_DETECT_V(L, "PathTracingIntegrator::L::UniformSampleAllLights");

		// 进行一次路径追踪采样
		Vector3f wIn;
		float pdf;
		BxDFType type;
		auto brdf = bsdf.SampleDirection(sampler->Get1D(), sampler->Get2D(), wo, &wIn,
			&pdf, BxDFType::BxDF_ALL, &type);
		NAN_DETECT_V(brdf, "PathTracingIntegrator::BSDF");
		INF_DETECT_V(brdf, "PathTracingIntegrator::BSDF");
		if (std::abs(pdf) == 0.f || brdf == glm::vec3(0))
		{
			break;
		}

		bool specular = (type & BxDF_SPECULAR);
		auto cosine = specular ? 1.0f : std::max(0.f, (type & BxDF_TRANSMISSION)
		   ? glm::dot(-N, wIn) : glm::dot(N, wIn));

		lightPath = specular || (type & BxDF_TRANSMISSION);
		currentRay = isec.SpawnRay(wIn);
		beta *= brdf * cosine / pdf;

		if (bounces > 3)
		{
			float q = std::max(.05f, 1.f - glm::length(beta));
			if (sampler->Get1D(0)< q)
				break;
			beta /= 1 - q;
		}

		NAN_DETECT_V(L, "PathTracingIntegrator::L");
		INF_DETECT_V(L, "PathTracingIntegrator::L");

		NAN_DETECT_V(beta, "PathTracingIntegrator::beta");
		INF_DETECT_V(beta, "PathTracingIntegrator::beta");
	}
	return L;
}
//
//glm::vec3 PathTracingIntegrator::eval_rec(const Ray& ray, Scene* scene,
//	Sampler* sampler, int level, bool specular)
//{
//	Spectrum L(0.f);
//	if (level == _maxDepth) return L;
//
//	SurfaceInteraction isec;
//	if (!scene->Intersect(ray, &isec))
//	{
//		if (scene->GetSkybox() == nullptr) return glm::vec3(0.f);
//		return scene->GetSkybox()->Evaluate(ray.dir);
//	}
//
//	Normal3f N = isec.GetNormal();
//	Point3f P = isec.GetHitPos();
//	auto E = isec.GetHitEntity();
//
//	// 如果是自发光物体就把发光项加上
//	if (specular)
//	{
//		L += isec.Le(-ray.dir);
//	}
//
//	BSDF bsdf(&isec);
//	isec.SetBSDF(&bsdf);
//	isec.GetHitEntity()->ComputeScatteringFunctions(isec, true);
//
//	// No bsdf function, means the object is transparent
//	if (bsdf.IsEmpty())
//	{
//		if (isec.GetHitEntity()->GetAreaLight() != nullptr)
//		{
//			return L;
//		}
//		return eval_rec(isec.SpawnRay(ray.dir), scene, sampler, level, specular);
//	}
//
//
//	// 进行一次路径追踪采样
//	Vector3f wIn;
//	float pdf;
//	BxDFType type;
//	auto brdf = bsdf.SampleDirection(sampler->Get1D(), sampler->Get2D(), -ray.dir, &wIn,
//		&pdf, BxDFType::BxDF_ALL, &type);
//	NAN_DETECT_V(brdf, "PathTracingIntegrator::BSDF");
//	INF_DETECT_V(brdf, "PathTracingIntegrator::BSDF");
//	if (std::abs(pdf) == 0.f || brdf == glm::vec3(0)) return L;
//	bool isSpecular = (type & BxDF_SPECULAR);
//
//	auto cosine = isSpecular ? 1.0f : std::max(0.f, (type & BxDF_TRANSMISSION)
//	   ? glm::dot(-N, wIn) : glm::dot(N, wIn));
//	auto Lindir = eval_rec(isec.SpawnRay(wIn), scene, sampler, level + 1, isSpecular) * brdf * cosine / pdf;
//
//	// 计算从光源采样的radiance
//	L += UniformSampleAllLights(isec, scene, sampler);
//
//	L += Lindir;
//
//	return L;
//}


Spectrum PathTracingIntegrator::UniformSampleAllLights(const SurfaceInteraction& isec, Scene* scene, Sampler* sampler)
{
	Spectrum L(0.f);
	// One sample for each light
	glm::vec2 sampleLight = sampler->Get2D();
	glm::vec2 sampleBSDF = sampler->Get2D();
	scene->ForEachLights([&](const crystal::Light* light) {
		if (light->Flux() == Spectrum(0.f) || light == isec.GetHitEntity()->GetAreaLight()) return;
		L += EsimateDirect(isec, scene, sampleLight, sampleBSDF, light, sampler);
	});
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
	SurfaceInfo surface = isec.GetSurfaceInfo(false);

	// Sample light source with MIS (Specular BSDF will not have value)
	Point3f lightPos;
	float pdf_light;
	auto Li_light = light->Sample_Li(surface, sampleLight, &lightPos, &pdf_light);

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

	NAN_DETECT_V(L, "PathTracingIntegrator::EsimateDirect::L::Light");
	INF_DETECT_V(L, "PathTracingIntegrator::EsimateDirect::L::Light");

	// Sample BSDF (Delta light should not have any value in BSDF sample)
	if (!light->IsDeltaLight())
	{
		float pdf_bsdf;
		BxDFType sampledType;
		Spectrum f = isec.GetBSDF()->SampleDirection(sampler->Get1D(), sampleBSDF, wo, &wi, &pdf_bsdf,
			sampleType, &sampledType);
		bool specularBSDF = (sampledType & BxDF_SPECULAR);

		if (f == Spectrum(0.f)) return L;

		float weight = 1.0f;
		if (!specularBSDF)
		{
			f *= std::max(0.f, glm::dot(N, wi));
			pdf_light = light->Pdf_Li(surface, wi);
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
		if (glm::isnan(f * Li * weight / pdf_bsdf )!=glm::bvec3(false))
		{
			printf("%lf %lf\n", pdf_bsdf, pdf_light);
		}
		if (Li != Spectrum(0.f))
		{
			L += f * Li * weight / pdf_bsdf;
		}
	}
	NAN_DETECT_V(L, "PathTracingIntegrator::EsimateDirect::L::BSDF");
	INF_DETECT_V(L, "PathTracingIntegrator::EsimateDirect::L::BSDF");
	return L;
}

#include "DirectLightingIntegrator.h"
#include <Core/Scene.h>
#include <Core/Geometry.h>
#include <Core/SurfaceInteraction.h>
#include <Core/Entities.h>
#include <Core/Utils.h>

#include <Textures/CubemapTexture.h>

#include <glm/gtx/transform.hpp>
#include <BSDFs/BSDF.h>
#include <Samplers/DefaultSampler.h>
#include <Lights/Light.h>
#include <Lights/AreaLight.h>

namespace crystal
{
	DirectLightingIntegrator::DirectLightingIntegrator(const std::shared_ptr<Sampler>& sampler,
		int threads, int maxDepth) : SamplerIntegrator(sampler, threads), _maxDepth(maxDepth)
	{}

	glm::vec3 DirectLightingIntegrator::Evaluate(const Ray& ray, Scene* scene, Sampler* sampler)
	{
		return eval_rec(ray, scene, sampler, 0, true);
	}

	glm::vec3 DirectLightingIntegrator::eval_rec(const Ray& ray, Scene* scene, Sampler* sampler,
		int level, bool specular)
	{
		Spectrum L(0.f);
		if (level == _maxDepth) return L;

		SurfaceInteraction isec;
		if (!scene->Intersect(ray, &isec))
		{
			if (scene->GetSkybox() == nullptr) return L;
			return scene->GetSkybox()->Evaluate(ray.dir);
		}

		// Get emitted radiance from hit surface to such direction
		L += isec.Le(-ray.dir);

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


		L += UniformSampleAllLights(isec, scene, sampler);


		return L;
	}

	Spectrum DirectLightingIntegrator::UniformSampleAllLights(const SurfaceInteraction& isec, Scene* scene, Sampler* sampler)
	{
		Spectrum L(0.f);
		// One sample for each light
		glm::vec2 sampleLight = sampler->Get2D();
		glm::vec2 sampleBSDF = sampler->Get2D();
		scene->ForEachLights([&](const crystal::Light* light) {
			if (light->Flux() == Spectrum(0.f)) return;
			L += EsimateDirect(isec, scene, sampleLight, sampleBSDF, light, sampler);
		});
		return L;
	}

	Spectrum DirectLightingIntegrator::EsimateDirect(const SurfaceInteraction& isec, Scene* scene,
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
				if (scene->IntersectTest(isec.SpawnRayTo(lightPos), 0, 1.f - EPS, light->GetAttachedObject()))
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
}
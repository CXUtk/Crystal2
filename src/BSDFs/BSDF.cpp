#include "BSDF.h"
#include <glm/glm.hpp>
#include <Core/SurfaceInteraction.h>
#include <Core/Utils.h>

BSDF::BSDF(const SurfaceInteraction* si) : _isec(si)
{}

void BSDF::AddBxDF(const std::shared_ptr<BxDF>& bxdf)
{
	assert(_numBxDF < MAX_BxDFs);
	_bxdfs[_numBxDF++] = bxdf;
}

BxDFType BSDF::Flags() const
{
	int flag = 0;
	for (int i = 0; i < _numBxDF; i++)
	{
		flag |= _bxdfs[i]->GetType();
	}
	return (BxDFType)flag;
}

Spectrum BSDF::DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const
{
	auto transform = _isec->GetInverseTNB();
	wOut = transform * wOut;
	wIn = transform * wIn;
	fixVector(wOut);
	fixVector(wIn);
	Spectrum L(0);
	bool reflect = wIn.y * wOut.y > 0;
	for (int i = 0; i < _numBxDF; i++)
	{
		auto& bxdf = _bxdfs[i];
		if ((reflect && bxdf->Contains(BxDFType::BxDF_REFLECTION)) || (!reflect && bxdf->Contains(BxDFType::BxDF_TRANSMISSION)))
		{
			L += bxdf->DistributionFunction(wOut, wIn);
		}
	}
	return L;
}

Spectrum BSDF::SampleDirection(float sampleBSDF, glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn,
	float* pdf, BxDFType flags, BxDFType* sampledType) const
{
	// 最大10个
	assert(_numBxDF < 10);
	int tot = 0;
	int idMap[10] = { 0 };

	// 按照概率分布均匀采样
	for (int i = 0; i < _numBxDF; i++)
	{
		auto& bxdf = _bxdfs[i];
		if (bxdf->Contains(flags))
		{
			idMap[tot] = i;
			++tot;
		}
	}

	if (!tot) return Spectrum(0.f);

	// Select one bxdf to sample wIn
	int v = (int)(std::min(0.9999999404f, sampleBSDF) * tot);
	std::shared_ptr<BxDF> selectedBxdf = _bxdfs[idMap[v]];

	*sampledType = selectedBxdf->GetType();

	wOut = _isec->GetInverseTNB() * wOut;
	fixVector(wOut);

	auto L = selectedBxdf->SampleDirection(sample, wOut, wIn, pdf, sampledType);
	if (L == Spectrum(0.f) || *pdf == 0.f) return Spectrum(0.f);

	if (!selectedBxdf->Contains(BxDFType::BxDF_SPECULAR))
	{
		for (int i = 0; i < _numBxDF; i++)
		{
			auto& bxdf = _bxdfs[i];
			if (bxdf != selectedBxdf && bxdf->Contains(flags))
			{
				*pdf += bxdf->Pdf(wOut, *wIn);
			}
		}

		bool reflect = wIn->y * wOut.y > 0;
		for (int i = 0; i < _numBxDF; i++)
		{
			auto& bxdf = _bxdfs[i];
			if (bxdf != selectedBxdf && bxdf->Contains(flags) &&
				((reflect && bxdf->Contains(BxDFType::BxDF_REFLECTION))
					|| (!reflect && bxdf->Contains(BxDFType::BxDF_TRANSMISSION))))
			{
				L += bxdf->DistributionFunction(wOut, *wIn);
			}
		}
	}
	*wIn = _isec->GetTNB() * (*wIn);
	fixVector(*wIn);


	if (L != Spectrum(0.f) && (glm::isnan(*wIn) != glm::bvec3(false) || glm::isinf(*wIn) != glm::bvec3(false)))
	{
		printf("Invalid value on BSDF::SampleDirection: wIn is [%lf, %lf, %lf]\n", wIn->x, wIn->y, wIn->z);
	}
	if (L != Spectrum(0.f) && (glm::isnan(L) != glm::bvec3(false) || glm::isinf(L) != glm::bvec3(false)))
	{
		printf("Invalid value on BSDF::SampleDirection: L is [%lf, %lf, %lf]\n", L.r, L.g, L.b);
	}

	*pdf /= tot;
	return L;
}

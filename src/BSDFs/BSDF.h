#pragma once
#include <glm/glm.hpp>
#include <Crystal2.h>
#include <memory>
#include <vector>

enum BxDFType : int {
    BxDF_REFLECTION = 1 << 0,
    BxDF_TRANSMISSION = 1 << 1,
    BxDF_GLOSSY = 1 << 2,
    BxDF_SPECULAR = 1 << 3,
    BxDF_DIFFUSE = 1 << 4,
    BxDF_ALL = 0x1F,
};

class BSDF {
public:
    BSDF(const SurfaceInteraction* si);
    int NumBxDFs() const { return _numBxDF; }
    void AddBxDF(const std::shared_ptr<BxDF>& bxdf);
    BxDFType Flags() const;

    bool IsEmpty() const { return _numBxDF == 0; }

    float Pdf(glm::vec3 wOut, glm::vec3 wIn, BxDFType flags) const;
    Spectrum DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const;
    Spectrum SampleDirection(float sampleBSDF, glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn, float* pdf, BxDFType flags, BxDFType* sampledType) const;

private:
    static constexpr int MAX_BxDFs = 8;
    const SurfaceInteraction* _isec = nullptr;
    std::shared_ptr<BxDF> _bxdfs[MAX_BxDFs];
    int _numBxDF = 0;
};

class BxDF {
public:
    BxDF(BxDFType type) : _bxdfType(type) {}
    virtual ~BxDF() = 0 {}

    /**
     * @brief Each wOut and wIn are normalized to TNB coordinates
    */
    virtual Spectrum DistributionFunction(glm::vec3 wOut, glm::vec3 wIn) const = 0;

    virtual float Pdf(glm::vec3 wOut, glm::vec3 wIn) const = 0;

    /**
     * @brief Each wOut and wIn are normalized to TNB coordinates
     * @param sample 
     * @param wOut 
     * @param wIn 
     * @param pdf 
     * @param sampledType 
     * @return 
    */
    virtual Spectrum SampleDirection(glm::vec2 sample, glm::vec3 wOut, glm::vec3* wIn, float* pdf, BxDFType* sampledType) const = 0;

    bool Contains(BxDFType type) const { return (_bxdfType & type); }

    BxDFType GetType() const { return _bxdfType; }

private:
    BxDFType _bxdfType;
};

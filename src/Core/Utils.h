#pragma once
#include <Crystal2.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

constexpr float EPS = 1e-6;

struct VertexData {
    Point3f Position;
    Normal3f Normal;
    Vector2f TexCoords;
};

// Solve for Ax = b
template <int N>
inline bool matSolve(glm::mat<N, N, glm::f32, glm::packed_highp> A,
    glm::vec<N, float, glm::packed_highp> b, glm::vec<N, float, glm::packed_highp>& x) {
    for (int i = 0; i < N; i++) {
        float absMax = A[i][i];
        int maxR = i;
        for (int j = i + 1; j < N; j++) {
            float abs = std::abs(A[i][j]);
            if (abs > absMax) {
                absMax = abs;
                maxR = j;
            }
        }
        if (absMax < EPS) return false;
        for (int j = i; j < N; j++) {
            std::swap(A[j][i], A[j][maxR]);
        }
        std::swap(b[i], b[maxR]);
        for (int j = i + 1; j < N; j++) {
            float K = A[i][j] / A[i][i];
            for (int k = i + 1; k < N; k++) {
                A[k][j] -= A[k][i] * K;
            }
            b[j] -= b[i] * K;
        }
    }
    for (int i = N - 1; i >= 0; i--) {
        for (int j = i + 1; j < N; j++) b[i] -= A[j][i] * b[j];
        b[i] /= A[i][i];
    }
    x = b;
    return true;
}

inline glm::mat3 adjoint(const glm::mat3& m, float invDet) {
    glm::mat3 Inverse;
    Inverse[0][0] = +(m[1][1] * m[2][2] - m[2][1] * m[1][2]) * invDet;
    Inverse[1][0] = -(m[1][0] * m[2][2] - m[2][0] * m[1][2]) * invDet;
    Inverse[2][0] = +(m[1][0] * m[2][1] - m[2][0] * m[1][1]) * invDet;
    Inverse[0][1] = -(m[0][1] * m[2][2] - m[2][1] * m[0][2]) * invDet;
    Inverse[1][1] = +(m[0][0] * m[2][2] - m[2][0] * m[0][2]) * invDet;
    Inverse[2][1] = -(m[0][0] * m[2][1] - m[2][0] * m[0][1]) * invDet;
    Inverse[0][2] = +(m[0][1] * m[1][2] - m[1][1] * m[0][2]) * invDet;
    Inverse[1][2] = -(m[0][0] * m[1][2] - m[1][0] * m[0][2]) * invDet;
    Inverse[2][2] = +(m[0][0] * m[1][1] - m[1][0] * m[0][1]) * invDet;
    return Inverse;
}

template<glm::length_t L, typename T, enum glm::qualifier Q>
inline T sqr(const glm::vec<L, T, Q>& v)
{
    return glm::dot(v, v);
}

inline glm::vec3 GetUnitVectorUsingCos(float cosTheta, float phi)
{
    float r = std::sqrt(1 - cosTheta * cosTheta);
    return glm::vec3(r * std::cos(phi), cosTheta, -r * std::sin(phi));
}

inline glm::vec3 GetUnitVector(float theta, float phi)
{
    float r = sin(theta);
    return glm::vec3(r * std::cos(phi), 1.0f - r * r, -r * std::sin(phi));
}

inline bool refract(glm::vec3 wo, float etaA, float etaB, glm::vec3* wt)
{
    float eta = etaA / etaB;
    wo *= -eta;
    auto sin2ThetaT = wo.x * wo.x + wo.z * wo.z;
    if (sin2ThetaT > 1.f) return false;
    wo.y = -std::sqrt(1.f - sin2ThetaT);
    *wt = wo;
    return true;
}

inline bool refract(const Vector3f& wo, const Vector3f& wh, float eta, glm::vec3* wt)
{
    auto cosThetaO = glm::dot(wo, wh);
    auto sin2ThetaO = std::sqrt(1.f - cosThetaO * cosThetaO);
    auto sin2ThetaI = eta * eta * sin2ThetaO;
    if (sin2ThetaI > 1.f) return false;

    auto cosThetaI = std::sqrt(1.f - sin2ThetaI);
    *wt = eta * -wo + (eta * cosThetaO - cosThetaI) * wh;
    return true;
}

inline glm::vec3 NextCosineUnitHemiSphere(glm::vec2 sample, float* pdf) {
    auto r = std::sqrt(sample.x);
    auto phi = sample.y * glm::two_pi<float>();

    auto x = r * std::cos(phi);
    auto z = r * std::sin(phi);
    auto y = std::sqrt(1.0f - r * r);
    *pdf = y / glm::pi<float>();
    return glm::vec3(x, y, z);
}

inline glm::vec3 NextUnitHemiSphere(glm::vec2 sample, float* pdf) {
    auto y = sample.x;
    auto phi = sample.y * glm::two_pi<float>();

    float r = std::sqrt(1 - y * y);
    auto x = r * std::cos(phi);
    auto z = r * std::sin(phi);

    *pdf = 1.0f / glm::two_pi<float>();
    return glm::vec3(x, y, z);
}


inline glm::vec3 NextUnitSphere(glm::vec2 sample, float* pdf)
{
    auto y = 2.f * sample.x - 1.f;
    auto phi = sample.y * glm::two_pi<float>();

    float r = std::sqrt(1 - y * y);
    auto x = r * std::cos(phi);
    auto z = r * std::sin(phi);

    *pdf = .5f / glm::two_pi<float>();
    return glm::vec3(x, y, z);
}

inline float square(float x) { return x * x; }


template<typename T>
inline void quickPrint(const T& value) {}

template<>
inline void quickPrint(const glm::vec3& value) 
{
    printf("%lf, %lf, %lf", value.x, value.y, value.z);
}

inline void fixVector(glm::vec3& vector)
{
    vector = glm::clamp(vector, glm::vec3(-1.0), glm::vec3(1.0));
}
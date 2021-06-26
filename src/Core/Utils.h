#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

constexpr float EPS = 1e-6;

struct VertexData {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
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

inline glm::vec3 NextCosineUnitHemiSphere(glm::vec2 sample, float& pdf) {
    auto r = std::sqrt(sample.x);
    auto phi = sample.y * glm::two_pi<float>();

    auto x = r * std::cos(phi);
    auto z = r * std::sin(phi);
    auto y = std::sqrt(1.0f - r * r);
    pdf = y / glm::pi<float>();
    return glm::vec3(x, y, z);
}

inline glm::vec3 NextUnitHemiSphere(glm::vec2 sample, float& pdf) {
    auto y = sample.x;
    auto phi = sample.y * glm::two_pi<float>();

    float r = std::sqrt(1 - y * y);
    auto x = r * std::cos(phi);
    auto z = r * std::sin(phi);

    pdf = 1.0f / glm::two_pi<float>();
    return glm::vec3(x, y, z);
}

#pragma once
#include <glm/glm.hpp>

constexpr float EPS = 1e-6;

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
        if (std::abs(A[i][maxR]) < EPS) return false;
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
        for (int j = i + 1; j < N; j++) b[i] -= v[j][i] * b[j];
        b[i] /= A[i][i];
    }
    x = b;
    return true;
}


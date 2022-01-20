#pragma once
#include <memory>
#include <random>
#include <glm/glm.hpp>

constexpr bool SHOW_DEBUG = true;

namespace SJson {
    // Loaders
    class SJsonNode;
}

namespace crystal
{
    class IIntersectable;
    class Entity;
    class Light;
    class AreaLight;
    class Integrator;
}

// Core
class Engine;
class RayTracer;

class SurfaceInteraction;
class SurfaceInfo;

class FrameBuffer;
class Scene;
class Camera;
class Shape;
class Accelerator;
class Material;
class BSDF;
class BxDF;

template<typename T>
class Texture;
class CubemapTexture;

// Shapes
class TriangleMesh;


// Type defines
using Point2f = glm::vec2;
using Point2i = glm::ivec2;
using Point3f = glm::vec3;
using Vector2f = glm::vec2;
using Vector3f = glm::vec3;
using Normal3f = glm::vec3;
using Spectrum = glm::vec3;
using Color3f = glm::vec3;

using JsonNode_CPTR = const SJson::SJsonNode*;
using Texture_RGB = Texture<Color3f>;
using Texture_Float = Texture<float>;
using Texture_Int = Texture<int>;

using RNG = std::mt19937;


/**
 * @brief Converts a shared pointer to a pointer of the object
 * @tparam T Object Type
 * @param ptr Target shared ponter
 * @return const reference to the object
*/
template<typename T>
inline T* ptr(const std::shared_ptr<T>& ptr) { return static_cast<T*>(ptr.get()); }

/**
 * @brief Converts a shared pointer to a pointer of the object
 * @tparam T Object Type
 * @param ptr Target shared ponter
 * @return const reference to the object
*/
template<typename T>
inline const T* cptr(const std::shared_ptr<T>& ptr) { return static_cast<const T*>(ptr.get()); }

/**
 * @brief Converts a unique pointer to a pointer of the object
 * @tparam T Object Type
 * @param ptr Target shared ponter
 * @return const reference to the object
*/
template<typename T>
inline T* ptr(const std::unique_ptr<T>& ptr) { return static_cast<T*>(ptr.get()); }


/**
 * @brief Converts a unique pointer to a const pointer of object
 * @tparam T Object Type
 * @param ptr Target shared ponter
 * @return const reference to the object
*/
template<typename T>
inline const T* cptr(const std::unique_ptr<T>& ptr) { return static_cast<const T*>(ptr.get()); }


template<glm::length_t L, typename T, enum glm::qualifier Q>
inline void reportNaN(const glm::vec<L, T, Q>& v, const char* title, int lineNum, const char* fileName)
{
    if (glm::isnan(v) != glm::vec<L, bool, Q>(false))
    {
        printf("NaN detected at file %s (file %s, line %d)\n", title, fileName, lineNum);
    }
}

inline void reportNaN(float v, const char* title, int lineNum, const char* fileName)
{
    if (std::isnan(v))
    {
        printf("NaN detected at file %s (file %s, line %d)\n", title, fileName, lineNum);
    }
}

template<glm::length_t L, typename T, enum glm::qualifier Q>
inline void reportINF(const glm::vec<L, T, Q>& v, const char* title, int lineNum, const char* fileName)
{
    if (glm::isinf(v) != glm::vec<L, bool, Q>(false))
    {
        printf("INF detected at %s (file %s, line %d)\n", title, fileName, lineNum);
    }
}

inline void reportINF(float v, const char* title, int lineNum, const char* fileName)
{
    if (std::isinf(v))
    {
        printf("INF detected at %s (file %s, line %d)\n", title, fileName, lineNum);
    }
}


// Micro definitions
#define NAN_DETECT_V(vec, name) if constexpr(SHOW_DEBUG) {reportNaN(vec, name, __LINE__, __FILE__);}
#define INF_DETECT_V(vec, name) if constexpr(SHOW_DEBUG) {reportINF(vec, name, __LINE__, __FILE__);}
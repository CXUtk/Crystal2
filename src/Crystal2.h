#pragma once
#include <memory>
#include <glm/glm.hpp>

namespace SJson {
    // Loaders
    class SJsonNode;
}

namespace crystal
{
    class Light;
}

// Core
class Engine;
class RayTracer;
class SurfaceInteraction;
class FrameBuffer;
class Scene;
class Integrator;
class Camera;
class Shape;
class Accelerator;
class Material;
class BSDF;
class BxDF;
class Prototype;
template<typename T>
class Texture;
class CubemapTexture;

// Shapes
class TriangleMesh;


// Type defines
using Point2f = glm::vec2;
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
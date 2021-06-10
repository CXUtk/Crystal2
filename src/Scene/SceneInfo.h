#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>

namespace Scene {
    enum class IntegratorType {
        WhittedStyle,
        PathTracer,
        PRTTracer,
    };

    enum class CameraType {
        Normal,
        Spherical,
        HDRSphere,
    };

    struct SceneObject {
        std::string Type;
        std::string FilePath;
        glm::vec3 Position, Scale;
    };

    struct CameraInfo {
        glm::vec3 Position, LookAt, Up;
        float FOV;
    };


    struct SceneInfo {
        int Width, Height;
        int SamplesPerPixel;
        IntegratorType IntegratorType;
        CameraInfo Camera;
        std::vector<SceneObject> Objects;
    };
}
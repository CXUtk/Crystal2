#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>

namespace config {
    struct SceneObject {
        std::string Type;
        std::string FilePath;
        glm::vec3 Position, Scale;
    };

    struct CameraInfo {
        std::string Type;
        glm::vec3 Position, LookAt, Up;
        float FOV;
    };

    struct SceneInfo {
        std::vector<SceneObject> Objects;
    };

    struct ConfigInfo {
        int Width, Height;
        int SamplesPerPixel;
        std::string SamplerType;
        std::string IntegratorType;
    };
}
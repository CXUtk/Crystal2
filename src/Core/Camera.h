#pragma once
#include <Loaders/SceneInfo.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <memory>

class Camera {
public:
    static std::shared_ptr<Camera> CreateCamera(const scene::SceneInfo& sceneInfo);

    virtual Ray GenerateRay(glm::vec2 pos) = 0;
};

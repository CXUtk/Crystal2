#pragma once
#include <Crystal2.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <memory>
#include <Core/Geometry.h>
#include <Core/ConfigInfo.h>

class Camera {
public:
    static std::shared_ptr<Camera> CreateCamera(const std::shared_ptr<SJsonNode>& cameraNode, const config::ConfigInfo& configInfo);

    virtual Ray GenerateRay(glm::vec2 pos) = 0;
};

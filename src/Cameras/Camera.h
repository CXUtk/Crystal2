#pragma once
#include <Crystal2.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <memory>
#include <Core/Geometry.h>
#include <Core/ConfigInfo.h>

class Camera {
public:
    static std::unique_ptr<Camera> CreateCamera(JsonNode_CPTR pCameraNode,
        const config::ConfigInfo& configInfo);

    virtual Ray GenerateRay(glm::vec2 pos) = 0;
};

#pragma once
#include <Crystal2.h>
#include "Texture.h"
#include "Texel.h"
#include "UVTexture.h"

class CubemapTexture {
    using texture2dRef = const std::shared_ptr<UVTexture<glm::vec3>>&;
    using texture2dPtr = std::shared_ptr<UVTexture<glm::vec3>>;
public:
    static std::shared_ptr<CubemapTexture> CreateCubemapTexture(const std::string& posx, const std::string& negx,
        const std::string& posy, const std::string& negy,
        const std::string& posz, const std::string& negz);

    glm::vec3 Evaluate(glm::vec3 dir) const;

private:
    CubemapTexture(texture2dRef posx, texture2dRef negx, texture2dRef posy,
        texture2dRef negy, texture2dRef posz, texture2dRef negz);

    texture2dPtr _faces[6];
};

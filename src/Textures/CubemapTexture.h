﻿#pragma once
#include <Crystal2.h>
#include "Texture.h"
#include "Texel.h"
#include "UVTexture.h"


class CubemapTexture {
public:
    virtual glm::vec3 Evaluate(glm::vec3 dir) const = 0;

protected:
    CubemapTexture() {}
};

class ImageCubemapTexture :public CubemapTexture {
    using texture2dRef = const std::shared_ptr<UVTexture<glm::vec3>>&;
    using texture2dPtr = std::shared_ptr<UVTexture<glm::vec3>>;
public:
    static std::unique_ptr<CubemapTexture> CreateImageCubemapTexture(const std::string& posx, const std::string& negx,
        const std::string& posy, const std::string& negy,
        const std::string& posz, const std::string& negz);

    glm::vec3 Evaluate(glm::vec3 dir) const override;

private:
    ImageCubemapTexture(texture2dRef posx, texture2dRef negx, texture2dRef posy,
        texture2dRef negy, texture2dRef posz, texture2dRef negz);

    texture2dPtr _faces[6];
};


class PureCubemapTexture : public CubemapTexture {
public:
    PureCubemapTexture(glm::vec3 color) :_color(color) {}
    glm::vec3 Evaluate(glm::vec3 dir) const { return _color; }

private:
    glm::vec3 _color;
};

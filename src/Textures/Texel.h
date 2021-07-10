#pragma once
#include "Texture.h"
#include <glm/glm.hpp>
#include <memory>
#include <string>

template <typename T>
class Texel2DUV {
public:
    virtual T GetTexel(glm::vec2 uv) const = 0;
    virtual ~Texel2DUV() = 0 {}
};


class ImageTexels : public Texel2DUV<glm::vec3> {
public:
    static std::shared_ptr<ImageTexels> CreateImageTexels(const std::string& fileName);
    glm::vec3 GetTexel(glm::vec2 uv) const override;
    ~ImageTexels() override;

private:
    ImageTexels(int width, int height, unsigned char* data);

    glm::vec3* _data;
    int _width, _height;
};

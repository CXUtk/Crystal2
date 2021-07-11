#include "Texel.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stbi/stb_image.h>

std::shared_ptr<ImageTexels> ImageTexels::CreateImageTexels(const std::string& fileName) {
    int width, height, components;
    unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &components, 3);
    if (!data) {
        throw std::exception(("Failed to load image: " + fileName).c_str());
    }
    auto p = std::shared_ptr<ImageTexels>(new ImageTexels(width, height, data));
    stbi_image_free(data);
    return p;
}

glm::vec3 ImageTexels::GetTexel(glm::vec2 uv) const {
    uv = glm::clamp(uv, glm::vec2(0.f, 0.f), glm::vec2(.99999f, .99999f));
    glm::ivec2 xy = glm::ivec2((int)(uv.x * _width), _height - (int)(uv.y * _height) - 1);
    return _data[xy.y * _width + xy.x];
}

ImageTexels::~ImageTexels() {
    delete[] _data;
}

static float byte2RGB(unsigned char x, bool gamma = true) {
    float v = (float)x / 255.f;
    return gamma ? std::pow(v, 2.2f) : v;
}

ImageTexels::ImageTexels(int width, int height, unsigned char* data) :_width(width), _height(height) {
    _data = new glm::vec3[_width * _height];
    for (int i = 0; i < _height; i++) {
        for (int j = 0; j < _width; j++) {
            _data[i * _width + j].r = byte2RGB(data[(i * _width + j) * 3]);
            _data[i * _width + j].g = byte2RGB(data[(i * _width + j) * 3 + 1]);
            _data[i * _width + j].b = byte2RGB(data[(i * _width + j) * 3 + 2]);
        }
    }
}

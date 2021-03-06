#include "FrameBuffer.h"
#include <glm/glm.hpp>
#include <Crystal2.h>
//FrameBuffer::FrameBuffer(glm::vec3* data, int width, int height) {
//    
//    for (int i = 0; i < height; i++) {
//        for (int j = 0; j < width; j++) {
//            auto c = data[i * width + j];
//            _color[(i * width + j) * 3] = (unsigned char)floor(glm::clamp(c.r, 0.f, 0.999f) * 256);
//            _color[(i * width + j) * 3 + 1] = (unsigned char)floor(glm::clamp(c.g, 0.f, 0.999f) * 256);
//            _color[(i * width + j) * 3 + 2] = (unsigned char)floor(glm::clamp(c.b, 0.f, 0.999f) * 256);
//        }
//    }
//}

FrameBuffer::FrameBuffer(int width, int height) :_width(width), _height(height) {
    _hdrData = std::unique_ptr<Pixel[]>(new Pixel[width * height]);
}

void FrameBuffer::AddSample(int x, int y, glm::vec3 hdr, float weight) {
    if (glm::isnan(hdr) != glm::bvec3(false)) {
        hdr = glm::vec3(0);
        printf("FrameBuffer::AddSample NAN detected\n");
    }
    if (glm::isinf(hdr) != glm::bvec3(false)) {
        hdr = glm::vec3(0);
        printf("FrameBuffer::AddSample INF detected\n");
    }
    auto& pixel = _hdrData[y * _width + x];
    pixel.color += hdr * weight;
    pixel.weight += weight;
}

void FrameBuffer::Clear() {
    for (int i = 0; i < _height; i++) {
        for (int j = 0; j < _width; j++) {
            memset(&_hdrData[i * _width + j], 0, sizeof(Pixel));
        }
    }
}

std::shared_ptr<unsigned char[]> FrameBuffer::GetImageDataRGB8(float gamma) const {
    auto data = std::shared_ptr<unsigned char[]>(new unsigned char[_width * _height * 3]);
    for (int i = 0; i < _height; i++) {
        for (int j = 0; j < _width; j++) {
            int orig = (_height - i - 1) * _width + j;
            int dest = i * _width + j;
            auto c = glm::pow(_hdrData[orig].color / _hdrData[orig].weight, glm::vec3(gamma));

            data[dest * 3] = (unsigned char)floor(glm::clamp(c.r, 0.f, 0.999f) * 256);
            data[dest * 3 + 1] = (unsigned char)floor(glm::clamp(c.g, 0.f, 0.999f) * 256);
            data[dest * 3 + 2] = (unsigned char)floor(glm::clamp(c.b, 0.f, 0.999f) * 256);
        }
    }
    return data;
}

std::shared_ptr<float[]> FrameBuffer::GetImageDataRGB32F() const
{
    auto data = std::shared_ptr<float[]>(new float[_width * _height * 3]);
    for (int i = 0; i < _height; i++)
    {
        for (int j = 0; j < _width; j++)
        {
            int orig = (_height - i - 1) * _width + j;
            int dest = i * _width + j;
            auto c = _hdrData[orig].color / _hdrData[orig].weight;

            data[dest * 3] = c.r;
            data[dest * 3 + 1] = c.g;
            data[dest * 3 + 2] = c.b;
        }
    }
    return data;
}

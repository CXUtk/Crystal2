#include "FrameBuffer.h"

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
    auto& pixel = _hdrData[y * _width + x];
    pixel.color += hdr * weight;
    pixel.weight += weight;
}

FrameBuffer::~FrameBuffer() {
    
}

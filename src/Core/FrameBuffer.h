#pragma once
#include <glm/glm.hpp>
#include <memory>
class FrameBuffer {
public:
    FrameBuffer(int width, int height);

    void AddSample(int x, int y, glm::vec3 hdr, float weight = 1.f);
    int Width() const { return _width; }
    int Height() const { return _height; }
    void Clear();

    std::shared_ptr<unsigned char[]> GetImageDataRGB8() const;
    std::shared_ptr<float[]> GetImageDataRGB32F() const;

private:
    struct Pixel {
        glm::vec3 color;
        float weight;
    };
    std::unique_ptr<Pixel[]> _hdrData;
    int _width, _height;
};
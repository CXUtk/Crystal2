#include "CubemapTexture.h"

std::shared_ptr<CubemapTexture> ImageCubemapTexture::CreateImageCubemapTexture(const std::string& posx, const std::string& negx,
    const std::string& posy, const std::string& negy,
    const std::string& posz, const std::string& negz) {
    auto tposx = std::make_shared<UVTexture<glm::vec3>>(ImageTexels::CreateImageTexels(posx));
    auto tnegx = std::make_shared<UVTexture<glm::vec3>>(ImageTexels::CreateImageTexels(negx));
    auto tposy = std::make_shared<UVTexture<glm::vec3>>(ImageTexels::CreateImageTexels(posy));
    auto tnegy = std::make_shared<UVTexture<glm::vec3>>(ImageTexels::CreateImageTexels(negy));
    auto tposz = std::make_shared<UVTexture<glm::vec3>>(ImageTexels::CreateImageTexels(posz));
    auto tnegz = std::make_shared<UVTexture<glm::vec3>>(ImageTexels::CreateImageTexels(negz));
    return std::shared_ptr<CubemapTexture>(new ImageCubemapTexture(tposx, tnegx, tposy, tnegy, tposz, tnegz));
}


struct CubeUV {
    int id;
    glm::vec2 uv;
};


inline CubeUV XYZ2CubeUV(const glm::vec3& p) {
    float ax = std::abs(p.x);
    float ay = std::abs(p.y);
    float az = std::abs(p.z);
    CubeUV c;
    if (ax >= ay && ax >= az) {
        // x face
        c = { p.x >= 0 ? 0 : 1, glm::vec2(-p.z / p.x, p.y / ax) };
    }
    else if (ay >= az) {
        // y face
        c = { p.y >= 0 ? 2 : 3, glm::vec2(p.x / ay, -p.z / p.y) };
    }
    else {
        // z face
        c = { p.z >= 0 ? 4 : 5, glm::vec2(p.x / p.z, p.y / az) };
    }
    if (glm::isnan(c.uv) != glm::bvec2(false)) {
        printf("%lf %lf %lf\n", p.x, p.y, p.z);
    }
    c.uv.x = c.uv.x * 0.5f + 0.5f;
    c.uv.y = c.uv.y * 0.5f + 0.5f;
    return c;
}


glm::vec3 ImageCubemapTexture::Evaluate(glm::vec3 dir) const {

    auto cube = XYZ2CubeUV(dir);
    return _faces[cube.id]->Evaluate(cube.uv);
}

ImageCubemapTexture::ImageCubemapTexture(texture2dRef posx, texture2dRef negx,
    texture2dRef posy, texture2dRef negy,
    texture2dRef posz, texture2dRef negz) {
    _faces[0] = posx;
    _faces[1] = negx;
    _faces[2] = posy;
    _faces[3] = negy;
    _faces[4] = posz;
    _faces[5] = negz;
}

#pragma once
#include <Crystal2.h>
#include <glm/glm.hpp>
#include <memory>
#include <Core/SurfaceInteraction.h>

#include "Texture.h"
#include "Texel.h"

template<typename T>
class UVTexture : public Texture<T> {
public:
    UVTexture(const std::shared_ptr<Texel2DUV<T>>& texel);
    T Evaluate(const SurfaceInteraction& isec) const override;
    ~UVTexture() override;

private:
    std::shared_ptr<Texel2DUV<T>> _texel2D;
};

template<typename T>
inline UVTexture<T>::UVTexture(const std::shared_ptr<Texel2DUV<T>>& texel) : _texel2D(texel) {
}

template<typename T>
inline T UVTexture<T>::Evaluate(const SurfaceInteraction& isec) const {
    return _texel2D->GetTexel(isec.GetUV());
}

template<typename T>
inline UVTexture<T>::~UVTexture() {
}

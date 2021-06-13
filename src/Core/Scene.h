#pragma once

#include "Loader/SceneInfo.h"
#include <memory>
class Scene {
public:
    static std::shared_ptr<Scene> CreateScene(const scene::SceneInfo& sceneInfo);
};
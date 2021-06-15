#pragma once

#include <Loaders/SceneInfo.h>
#include <Shapes/Shape.h>
#include <memory>
class Scene {
public:
    static std::shared_ptr<Scene> CreateScene(const scene::SceneInfo& sceneInfo);

private:
    Scene();
    ~Scene();

    std::vector<std::shared_ptr<Shape>> _sceneObjects;
};
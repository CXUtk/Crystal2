#pragma once
#include <Crystal2.h>
#include <memory>
#include <vector>

class Scene {
public:
    static std::shared_ptr<Scene> CreateScene(const std::shared_ptr<SJsonNode>& sceneNode);

private:
    Scene();
    ~Scene();

    std::vector<std::shared_ptr<Shape>> _sceneObjects;
};
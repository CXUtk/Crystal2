#pragma once
#include <Crystal2.h>
#include <memory>
#include <vector>
#include <Core/ConfigInfo.h>
#include <Core/Geometry.h>

class Scene {
public:
    static std::shared_ptr<Scene> CreateScene(const std::shared_ptr<SJsonNode>& sceneNode, const config::ConfigInfo& configInfo);
    ~Scene();

    bool Intersect(const Ray& ray, SurfaceInteraction* info) const;

private:
    Scene();
    std::vector<std::shared_ptr<Shape>> _sceneObjects;
    std::unique_ptr<Accelerator> _accelStructure;
};
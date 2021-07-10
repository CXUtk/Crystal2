#pragma once
#include <Crystal2.h>
#include <memory>
#include <vector>
#include <map>
#include <Core/ConfigInfo.h>
#include <Core/Geometry.h>

class Scene {
public:
    static std::shared_ptr<Scene> CreateScene(const std::shared_ptr<SJson::SJsonNode>& sceneNode,
        const config::ConfigInfo& configInfo);
    ~Scene();

    bool Intersect(const Ray& ray, SurfaceInteraction* info) const;
    bool IntersectTest(const Ray& ray, float tMin, float tMax) const;
    std::vector<std::shared_ptr<Light>> GetLights() const { return _lights; }

    std::shared_ptr<Texture<glm::vec3>> GetTextureByName(const std::string& name) const;

private:
    Scene();
    std::vector<std::shared_ptr<Shape>> _sceneObjects;
    std::unique_ptr<Accelerator> _accelStructure;
    std::vector<std::shared_ptr<TriangleMesh>> _triangleMeshes;
    std::vector<std::shared_ptr<Light>> _lights;

    std::map<std::string, std::shared_ptr<Texture<glm::vec3>>> _defaultTextures;


    // Load from scene file
    void loadTextures(const std::shared_ptr<SJson::SJsonNode>& texturesNode, const config::ConfigInfo& configInfo);
    void loadObjects(const std::shared_ptr<SJson::SJsonNode>& objectsNode, const config::ConfigInfo& configInfo);
    std::vector<std::shared_ptr<Shape>> parse_shape(const std::shared_ptr<Prototype>& prototype,
        const std::shared_ptr<SJson::SJsonNode>& shapeNode);
};

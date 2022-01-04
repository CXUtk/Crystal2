#pragma once
#include <Crystal2.h>
#include <memory>
#include <vector>
#include <map>
#include <functional>

#include <Core/ConfigInfo.h>
#include <Core/Geometry.h>

class Scene
{
public:
    static std::unique_ptr<Scene> CreateScene(JsonNode_CPTR pSceneNode,
        const config::ConfigInfo& configInfo);
    Scene();
    ~Scene();

    bool Intersect(const Ray& ray, SurfaceInteraction* info) const;
    bool IntersectTest(const Ray& ray, float tMin, float tMax,
        const crystal::IIntersectable* ignoreShape) const;

    void ForEachLights(std::function<void(const crystal::Light*)> action) const;


    std::vector<const crystal::Entity*> GetObjects() const
    {
        std::vector<const crystal::Entity*> objects;
        for (auto& object : _sceneObjects)
        {
            objects.push_back(cptr(object));
        }
        return objects;
    }

    const Texture_RGB* GetTextureByName(const std::string& name) const;
    const CubemapTexture* GetSkybox() const { return cptr(_skybox); }

private:
    std::vector<std::shared_ptr<crystal::Entity>> _sceneObjects;
    std::unique_ptr<Accelerator> _accelStructure;
    std::vector<std::shared_ptr<crystal::Light>> _lights;
    std::vector<std::shared_ptr<Shape>> _shapes;
    std::vector<std::shared_ptr<Material>> _materials;
    std::vector<std::shared_ptr<TriangleMesh>> _triangleMeshes;

    std::map<std::string, std::shared_ptr<Texture_RGB>> _defaultTextures;
    std::unique_ptr<CubemapTexture> _skybox;

    // Load from scene file
    void loadTextures(JsonNode_CPTR pTexturesNode, const config::ConfigInfo& configInfo);
    void loadObjects(JsonNode_CPTR pObjectsNode, const config::ConfigInfo& configInfo);
    void loadSkybox(JsonNode_CPTR pSkyboxNode, const config::ConfigInfo& configInfo);

    std::vector<const Shape*> createShapes(JsonNode_CPTR pShapeNode);
    const Material* createMaterial(JsonNode_CPTR pNode);
    const crystal::AreaLight* createAreaLight(JsonNode_CPTR pNode, const Shape* shape);
    void createGeometricEntity(const Shape* shape, const Material* material, const crystal::AreaLight* areaLight);
};

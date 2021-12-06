#pragma once
#include <Crystal2.h>
#include <memory>
#include <vector>
#include <map>
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
    bool IntersectTest(const Ray& ray, float tMin, float tMax) const;
    std::vector<const Light*> GetLights() const
    {
        std::vector<const Light*> lights;
        for (auto& light : _lights)
        {
            lights.push_back(cptr(light));
        }
        return lights;
    }

    std::vector<const Shape*> GetObjects() const
    {
        std::vector<const Shape*> shapes;
        for (auto& shape : _sceneObjects)
        {
            shapes.push_back(cptr(shape));
        }
        return shapes;
    }

    const Texture_RGB* GetTextureByName(const std::string& name) const;
    const CubemapTexture* GetSkybox() const { return cptr(_skybox); }

private:
    std::vector<std::shared_ptr<Shape>> _sceneObjects;
    std::unique_ptr<Accelerator> _accelStructure;
    std::vector<std::shared_ptr<TriangleMesh>> _triangleMeshes;
    std::vector<std::unique_ptr<Light>> _lights;
    std::vector<std::unique_ptr<Prototype>> _prototypes;

    std::map<std::string, std::shared_ptr<Texture_RGB>> _defaultTextures;
    std::unique_ptr<CubemapTexture> _skybox;

    // Load from scene file
    void loadTextures(JsonNode_CPTR pTexturesNode, const config::ConfigInfo& configInfo);
    void loadObjects(JsonNode_CPTR pObjectsNode, const config::ConfigInfo& configInfo);
    void loadSkybox(JsonNode_CPTR pSkyboxNode, const config::ConfigInfo& configInfo);
    std::vector<std::shared_ptr<Shape>> parse_shape(Prototype* prototype,
        JsonNode_CPTR pShapeNode);
};

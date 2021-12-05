#include "Scene.h"
#include <cassert>
#include <SJson/SJson.h>
#include <Shapes/Shape.h>
#include <Loaders/JsonLoader.h>
#include <Accelerators/Accelerator.h>
#include <Core/Utils.h>
#include <Core/Prototype.h>

#include <Shapes/Triangle.h>
#include <Shapes/Sphere.h>
#include <Shapes/TriangleMesh.h>
#include <Lights/PointLight.h>

#include <Textures/Texture.h>
#include <Textures/Texel.h>
#include <Textures/UVTexture.h>
#include <Textures/CubemapTexture.h>

std::unique_ptr<Scene> Scene::CreateScene(JsonNode_CPTR pSceneNode,
    const config::ConfigInfo& configInfo) {
    assert(pSceneNode->GetType() == SJson::SJsonNodeType::JSON_OBJECT);
    auto scene = std::make_unique<Scene>();

    // scene->_lights.push_back(std::make_shared<PointLight>(glm::vec3(0, 20, 0), glm::vec3(10000.f)));

    scene->loadTextures(pSceneNode->GetMember("Texture"), configInfo);
    scene->loadObjects(pSceneNode->GetMember("Objects"), configInfo);
    if (pSceneNode->HasMember("Skybox")) {
        scene->loadSkybox(pSceneNode->GetMember("Skybox"), configInfo);
    }

    scene->_accelStructure = Accelerator::GetAccelerator(configInfo.AccelType);
    scene->_accelStructure->Build(scene->GetObjects());
    return scene;
}

Scene::~Scene() {
}

bool Scene::Intersect(const Ray& ray, SurfaceInteraction* info) const {
    return _accelStructure->Intersect(ray, info);
}

bool Scene::IntersectTest(const Ray& ray, float tMin, float tMax) const {
    return _accelStructure->IntersectTest(ray, tMin, tMax);
}

std::shared_ptr<Texture<glm::vec3>> Scene::GetTextureByName(const std::string& name) const {
    auto it = _defaultTextures.find(name);
    if (it != _defaultTextures.end()) {
        return it->second;
    }
    throw std::exception("Texture name doesn't exist");
}

Scene::Scene() {
    _skybox = nullptr;
}

void Scene::loadTextures(JsonNode_CPTR pTexturesNode, const config::ConfigInfo& configInfo)
{
    assert(pTexturesNode->GetType() == SJson::SJsonNodeType::JSON_OBJECT);

    pTexturesNode->ForEachProperties([&](const std::string& name, const SJson::SJsonNode* node) {
        auto path = node->GetMember("Path")->GetString();
        _defaultTextures[name] = std::make_shared<UVTexture<glm::vec3>>(ImageTexels::CreateImageTexels(path));
    });
}

void Scene::loadObjects(JsonNode_CPTR pObjectsNode, const config::ConfigInfo& configInfo)
{
    assert(pObjectsNode->GetType() == SJson::SJsonNodeType::JSON_ARRAY);
    pObjectsNode->ForEachProperties([&](const std::string& name, const SJson::SJsonNode* node) {
        assert(node->GetType() == SJson::SJsonNodeType::JSON_OBJECT);
        auto typeString = node->GetMember("Type")->GetString();
        if (typeString == "Geometry") {
            auto prototype = Prototype::CreatePrototype(node, this);
            for (auto& s : parse_shape(prototype, node->GetMember("Shape"))) {
                _sceneObjects.push_back(s);
            }
        }
        else if (typeString == "Light") {

        }
        else {
            throw std::invalid_argument("Invalid Object Type!");
        }
        });
}

void Scene::loadSkybox(JsonNode_CPTR pSkyboxNode, const config::ConfigInfo& configInfo)
{
    if (pSkyboxNode->IsNull()) return;
    //_skybox = std::make_shared<PureCubemapTexture>(glm::vec3(.5f));
    // Path to skybox folder
    auto path = pSkyboxNode->GetMember("Path")->GetString();

    std::string suffix[6] = { "posx.jpg", "negx.jpg", "posy.jpg", "negy.jpg", "posz.jpg", "negz.jpg" };
    std::string paths[6];
    for (int i = 0; i < 6; i++) {
        paths[i] = path + "/" + suffix[i];
    }
    _skybox = ImageCubemapTexture::CreateImageCubemapTexture(paths[0], paths[1], paths[2], 
        paths[3], paths[4], paths[5]);
}

std::vector<std::unique_ptr<Shape>> Scene::parse_shape(Prototype* prototype,
    JsonNode_CPTR pShapeNode)
{
    assert(pShapeNode->GetType() == SJson::SJsonNodeType::JSON_OBJECT);
    auto shapeType = pShapeNode->GetMember("Type")->GetString();
    std::vector<std::shared_ptr<Shape>> shapes;
    if (shapeType == "Sphere")
    {
        shapes.push_back(Sphere::CreateSphere(prototype, pShapeNode));
    }
    else if (shapeType == "TriangleMesh")
    {
        auto mesh = TriangleMesh::CreateTriangleMesh(prototype, pShapeNode);
        this->_triangleMeshes.push_back(mesh);
        shapes = mesh->GetTriangles();
    }
    else
    {
        throw std::invalid_argument("Invalid Shape Type!");
    }
    return shapes;
}

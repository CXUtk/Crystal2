#include "Scene.h"
#include <SJson/SJson.h>
#include <cassert>
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

std::shared_ptr<Scene> Scene::CreateScene(const std::shared_ptr<SJson::SJsonNode>& sceneNode, const config::ConfigInfo& configInfo) {
    assert(sceneNode->GetType() == SJson::SJsonNodeType::JSON_OBJECT);
    auto scene = std::shared_ptr<Scene>(new Scene());

    scene->_lights.push_back(std::make_shared< PointLight>(glm::vec3(0, 4, 0), glm::vec3(100.f)));

    scene->loadTextures(sceneNode->GetMember("Textures"), configInfo);
    scene->loadObjects(sceneNode->GetMember("Objects"), configInfo);

    scene->_accelStructure = Accelerator::GetAccelerator(configInfo.AccelType);
    scene->_accelStructure->Build(scene->_sceneObjects);
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
}

void Scene::loadTextures(const std::shared_ptr<SJson::SJsonNode>& texturesNode, const config::ConfigInfo& configInfo) {
    assert(texturesNode->GetType() == SJson::SJsonNodeType::JSON_OBJECT);

    for (auto it = texturesNode->beginProperties(); it != texturesNode->endProperties(); it++) {
        auto& node = it->second;
        auto path = node->GetMember("Path")->GetString();
        _defaultTextures[it->first] = std::make_shared<UVTexture<glm::vec3>>(ImageTexels::CreateImageTexels(path));
    }
}

void Scene::loadObjects(const std::shared_ptr<SJson::SJsonNode>& objectsNode, const config::ConfigInfo& configInfo) {
    assert(objectsNode->GetType() == SJson::SJsonNodeType::JSON_ARRAY);
    for (auto it = objectsNode->begin(); it != objectsNode->end(); it++) {
        auto& node = (*it);
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
    }
}

std::vector<std::shared_ptr<Shape>> Scene::parse_shape(const std::shared_ptr<Prototype>& prototype, const std::shared_ptr<SJson::SJsonNode>& shapeNode) {
    assert(shapeNode->GetType() == SJson::SJsonNodeType::JSON_OBJECT);
    auto shapeType = shapeNode->GetMember("Type")->GetString();
    std::vector<std::shared_ptr<Shape>> shapes;
    if (shapeType == "Sphere") {
        shapes.push_back(Sphere::CreateSphere(prototype, shapeNode));
    }
    else if (shapeType == "TriangleMesh") {
        auto mesh = TriangleMesh::CreateTriangleMesh(prototype, shapeNode);
        this->_triangleMeshes.push_back(mesh);
        shapes = mesh->GetTriangles();
    }
    else {
        throw std::invalid_argument("Invalid Shape Type!");
    }
    return shapes;
}

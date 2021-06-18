#include "Scene.h"
#include <SJson/SJson.h>
#include <cassert>
#include <Shapes/Shape.h>
#include <Loaders/JsonLoader.h>
#include <Shapes/Sphere.h>
#include <Accelerators/Accelerator.h>

static std::shared_ptr<Shape> parse_shape(const std::shared_ptr<SJsonNode>& shapeNode) {
    assert(shapeNode->GetType() == SJsonNodeType::JSON_OBJECT);
    auto shapeType = shapeNode->GetMember("Type")->GetString();
    if (shapeType == "Sphere") {
        auto pos = loader::parse_vec3(shapeNode->GetMember("Position"));
        auto r = shapeNode->GetMember("Radius")->GetFloat();
        return std::make_shared<Sphere>(pos, r, glm::vec3(0));
    }
    else {
        throw std::invalid_argument("Invalid Shape Type!");
    }
    return nullptr;
}

std::shared_ptr<Scene> Scene::CreateScene(const std::shared_ptr<SJsonNode>& sceneNode, const config::ConfigInfo& configInfo) {
    assert(sceneNode->GetType() == SJsonNodeType::JSON_ARRAY);
    auto scene = std::shared_ptr<Scene>(new Scene());

    for (auto it = sceneNode->begin(); it != sceneNode->end(); it++) {
        auto& node = (*it);
        assert(node->GetType() == SJsonNodeType::JSON_OBJECT);
        auto typeString = node->GetMember("Type")->GetString();
        if (typeString == "Geometry") {
            scene->_sceneObjects.push_back(parse_shape(node->GetMember("Shape")));
        }
        else {
            throw std::invalid_argument("Invalid Object Type!");
        }
    }

    scene->_accelStructure = Accelerator::GetAccelerator(configInfo.AccelType);
    scene->_accelStructure->Build(scene->_sceneObjects);
    return scene;
}

Scene::~Scene() {
}

bool Scene::Intersect(const Ray& ray, SurfaceInteraction* info) const {
    return _accelStructure->Intersect(ray, info);
}

Scene::Scene() {
}

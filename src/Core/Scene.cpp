#include "Scene.h"
#include <SJson/SJson.h>
#include <cassert>
#include <Shapes/Shape.h>
#include <Loaders/JsonLoader.h>
#include <Shapes/Sphere.h>

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

std::shared_ptr<Scene> Scene::CreateScene(const std::shared_ptr<SJsonNode>& sceneNode) {
    assert(sceneNode->GetType() == SJsonNodeType::JSON_ARRAY);

    auto scene = std::shared_ptr<Scene>(new Scene());

    std::vector<std::shared_ptr<Shape>> sceneObjects;

    for (auto it = sceneNode->begin(); it != sceneNode->end(); it++) {
        auto& node = (*it);
        assert(node->GetType() == SJsonNodeType::JSON_OBJECT);
        auto typeString = node->GetMember("Type")->GetString();
        if (typeString == "Geometry") {
            sceneObjects.push_back(parse_shape(node->GetMember("Shape")));
        }
        else {
            throw std::invalid_argument("Invalid Object Type!");
        }
    }
    return scene;
}

Scene::~Scene() {
}

Scene::Scene() {
}

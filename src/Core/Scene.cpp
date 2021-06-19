#include "Scene.h"
#include <SJson/SJson.h>
#include <cassert>
#include <Shapes/Shape.h>
#include <Loaders/JsonLoader.h>
#include <Accelerators/Accelerator.h>
#include <Core/Utils.h>

#include <Shapes/Triangle.h>
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
static std::vector<VertexData> vertices;
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

    vertices.push_back({ glm::vec3(-3, -1, 3), glm::vec3(0, 1, 0), glm::vec2(0, 0) });
    vertices.push_back({ glm::vec3(-3, -1, -3), glm::vec3(0, 1, 0), glm::vec2(0, 1) });
    vertices.push_back({ glm::vec3(3, -1, -3), glm::vec3(0, 1, 0), glm::vec2(1, 1) });
    vertices.push_back({ glm::vec3(3, -1, 3), glm::vec3(0, 1, 0), glm::vec2(1, 0) });

    scene->_sceneObjects.push_back(std::make_shared<Triangle>(&vertices[0], &vertices[1], &vertices[2]));
    scene->_sceneObjects.push_back(std::make_shared<Triangle>(&vertices[0], &vertices[2], &vertices[3]));

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

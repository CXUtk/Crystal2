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

std::shared_ptr<Scene> Scene::CreateScene(const std::shared_ptr<SJson::SJsonNode>& sceneNode, const config::ConfigInfo& configInfo) {
    assert(sceneNode->GetType() == SJson::SJsonNodeType::JSON_ARRAY);
    auto scene = std::shared_ptr<Scene>(new Scene());

    scene->_lights.push_back(std::make_shared< PointLight>(glm::vec3(0, 4, 0), glm::vec3(100.f)));

    for (auto it = sceneNode->begin(); it != sceneNode->end(); it++) {
        auto& node = (*it);
        assert(node->GetType() == SJson::SJsonNodeType::JSON_OBJECT);
        auto typeString = node->GetMember("Type")->GetString();
        if (typeString == "Geometry") {
            auto prototype = Prototype::CreatePrototype(node);
            for (auto& s : scene->parse_shape(prototype, node->GetMember("Shape"))) {
                scene->_sceneObjects.push_back(s);
            }
        }
        else if (typeString == "Light") {

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

bool Scene::IntersectTest(const Ray& ray, float tMin, float tMax) const {
    return _accelStructure->IntersectTest(ray, tMin, tMax);
}

Scene::Scene() {
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

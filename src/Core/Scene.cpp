#include "Scene.h"
#include <SJson/SJson.h>
#include <cassert>
#include <Shapes/Shape.h>
#include <Loaders/JsonLoader.h>
#include <Accelerators/Accelerator.h>
#include <Core/Utils.h>

#include <Shapes/Triangle.h>
#include <Shapes/Sphere.h>
#include <Shapes/TriangleMesh.h>


std::shared_ptr<Scene> Scene::CreateScene(const std::shared_ptr<SJsonNode>& sceneNode, const config::ConfigInfo& configInfo) {
    assert(sceneNode->GetType() == SJsonNodeType::JSON_ARRAY);
    auto scene = std::shared_ptr<Scene>(new Scene());

    for (auto it = sceneNode->begin(); it != sceneNode->end(); it++) {
        auto& node = (*it);
        assert(node->GetType() == SJsonNodeType::JSON_OBJECT);
        auto typeString = node->GetMember("Type")->GetString();
        if (typeString == "Geometry") {
            for (auto& s : scene->parse_shape(node->GetMember("Shape"))) {
                scene->_sceneObjects.push_back(s);
            }
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

std::vector<std::shared_ptr<Shape>> Scene::parse_shape(const std::shared_ptr<SJsonNode>& shapeNode) {
    assert(shapeNode->GetType() == SJsonNodeType::JSON_OBJECT);
    auto shapeType = shapeNode->GetMember("Type")->GetString();
    std::vector<std::shared_ptr<Shape>> shapes;
    if (shapeType == "Sphere") {
        shapes.push_back(Sphere::CreateSphere(shapeNode));
    }
    else if (shapeType == "TriangleMesh") {
        auto mesh = TriangleMesh::CreateTriangleMesh(shapeNode);
        this->_triangleMeshes.push_back(mesh);
        shapes = mesh->GetTriangles();
    }
    else {
        throw std::invalid_argument("Invalid Shape Type!");
    }
    return shapes;
}

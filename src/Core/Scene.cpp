#include "Scene.h"
#include <cassert>
#include <SJson/SJson.h>
#include <Shapes/Shape.h>
#include <Loaders/JsonLoader.h>
#include <Accelerators/Accelerator.h>
#include <Core/Utils.h>
#include <Core/Entities.h>

#include <Shapes/Triangle.h>
#include <Shapes/Sphere.h>
#include <Shapes/TriangleMesh.h>
#include <Lights/PointLight.h>
#include <Lights/AreaLight.h>
#include <Materials/Material.h>

#include <Textures/Texture.h>
#include <Textures/Texel.h>
#include <Textures/UVTexture.h>
#include <Textures/CubemapTexture.h>

std::unique_ptr<Scene> Scene::CreateScene(JsonNode_CPTR pSceneNode,
	const config::ConfigInfo& configInfo)
{
	assert(pSceneNode->GetType() == SJson::SJsonNodeType::JSON_OBJECT);
	auto scene = std::make_unique<Scene>();

	// scene->_lights.push_back(std::make_shared<PointLight>(glm::vec3(0, 20, 0), glm::vec3(10000.f)));

	scene->loadTextures(pSceneNode->GetMember("Textures"), configInfo);
	scene->loadObjects(pSceneNode->GetMember("Objects"), configInfo);
	if (pSceneNode->HasMember("Skybox"))
	{
		scene->loadSkybox(pSceneNode->GetMember("Skybox"), configInfo);
	}

	scene->_accelStructure = Accelerator::GetAccelerator(configInfo.AccelType);
	scene->_accelStructure->Build(scene->GetObjects());
	return scene;
}

Scene::~Scene()
{}

bool Scene::Intersect(const Ray& ray, SurfaceInteraction* info) const
{
	return _accelStructure->Intersect(ray, info);
}

bool Scene::IntersectTest(const Ray& ray, float tMin, float tMax) const
{
	return _accelStructure->IntersectTest(ray, tMin, tMax);
}

const Texture_RGB* Scene::GetTextureByName(const std::string& name) const
{
	auto it = _defaultTextures.find(name);
	if (it != _defaultTextures.end())
	{
		return cptr(it->second);
	}
	throw std::exception("Texture name doesn't exist");
}

Scene::Scene()
{
	_skybox = nullptr;
}

void Scene::loadTextures(JsonNode_CPTR pTexturesNode, const config::ConfigInfo& configInfo)
{
	assert(pTexturesNode->GetType() == SJson::SJsonNodeType::JSON_OBJECT);

	pTexturesNode->ForEachProperties([&](const std::string& name, const SJson::SJsonNode* node) {
		auto path = node->GetMember("Path")->GetString();
		_defaultTextures[name] = std::make_shared<TextureUV_RGB>(ImageTexels::CreateImageTexels(path));
	});
}

void Scene::loadObjects(JsonNode_CPTR pObjectsNode, const config::ConfigInfo& configInfo)
{
	assert(pObjectsNode->GetType() == SJson::SJsonNodeType::JSON_ARRAY);
	pObjectsNode->ForEachElements([&](JsonNode_CPTR pNode) {
		assert(pNode->GetType() == SJson::SJsonNodeType::JSON_OBJECT);
		auto typeString = pNode->GetMember("Type")->GetString();
		if (typeString == "Geometry")
		{
			auto shape = createShape(pNode->GetMember("Shape"));
			const Material* material = nullptr;
			const crystal::AreaLight* areaLight = nullptr;
			if (pNode->HasMember("Material"))
			{
				material = createMaterial(pNode->GetMember("Material"));
			}
			if (pNode->HasMember("AreaLight"))
			{
				areaLight = createAreaLight(pNode->GetMember("AreaLight"), shape);
			}
			createGeometricEntity(shape, material, areaLight);
		}
		else if (typeString == "Light")
		{
			auto light = crystal::Light::CreateLight(pNode, this);
			_lights.push_back(light);
		}
		else
		{
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
	for (int i = 0; i < 6; i++)
	{
		paths[i] = path + "/" + suffix[i];
	}
	_skybox = ImageCubemapTexture::CreateImageCubemapTexture(paths[0], paths[1], paths[2],
		paths[3], paths[4], paths[5]);
}

const Shape* Scene::createShape(JsonNode_CPTR pShapeNode)
{
	assert(pShapeNode->GetType() == SJson::SJsonNodeType::JSON_OBJECT);
	auto shapeType = pShapeNode->GetMember("Type")->GetString();
	if (shapeType == "Sphere")
	{
		auto sphere = Sphere::CreateSphere(pShapeNode);
		_shapes.push_back(sphere);
		return cptr(sphere);
	}
	else if (shapeType == "TriangleMesh")
	{
		auto mesh = TriangleMesh::CreateTriangleMesh(pShapeNode);
		_shapes.push_back(mesh);
		return cptr(mesh);
	}
	else
	{
		throw std::invalid_argument("Invalid Shape Type!");
	}
}

const Material* Scene::createMaterial(JsonNode_CPTR pNode)
{
	auto material = Material::CreateMaterial(pNode, this);
	_materials.push_back(material);
	return cptr(material);
}

const crystal::AreaLight* Scene::createAreaLight(JsonNode_CPTR pNode, const Shape* shape)
{
	auto areaLight = crystal::Light::CreateAreaLight(pNode, shape, this);
	_lights.push_back(areaLight);
	return cptr(areaLight);
}

void Scene::createGeometricEntity(const Shape* shape, const Material* material, const crystal::AreaLight* areaLight)
{
	_sceneObjects.push_back(std::make_shared<crystal::GeometricEntity>(shape, material, areaLight));
}

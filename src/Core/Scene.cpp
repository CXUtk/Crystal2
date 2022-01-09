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
#include <Shapes/Disk.h>
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
	printf("Created Acceleration Structure: %s\n", configInfo.AccelType.c_str());
	auto sceneEntities = scene->GetObjects();
	auto objects = std::vector<const crystal::IIntersectable*>(sceneEntities.begin(),
		sceneEntities.end());
	scene->_accelStructure->Build(objects);
	return scene;
}

Scene::~Scene()
{}

bool Scene::Intersect(const Ray& ray, SurfaceInteraction* info) const
{
	return _accelStructure->Intersect(ray, info);
}

bool Scene::IntersectTest(const Ray& ray, float tMin, float tMax, 
	const crystal::IIntersectable* ignoreShape) const
{
	return _accelStructure->IntersectTest(ray, ignoreShape, tMin, tMax);
}

void Scene::ForEachLights(std::function<void(const crystal::Light*)> action) const
{
	for (auto& light : _lights)
	{
		action(cptr(light));
	}
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
			auto shapes = createShapes(pNode->GetMember("Shape"));
			const Material* material = nullptr;
			if (pNode->HasMember("Material"))
			{
				material = createMaterial(pNode->GetMember("Material"));
			}
			bool hasLight = pNode->HasMember("AreaLight");
			for (auto& shape : shapes)
			{
				createGeometricEntity(shape, material, hasLight ?
					createAreaLight(pNode->GetMember("AreaLight"), shape)
				: nullptr);
			}
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

std::vector<const Shape*> Scene::createShapes(JsonNode_CPTR pShapeNode)
{
	assert(pShapeNode->GetType() == SJson::SJsonNodeType::JSON_OBJECT);
	auto shapeType = pShapeNode->GetMember("Type")->GetString();
	std::vector<const Shape*> shapesPtr;
	if (shapeType == "Sphere")
	{
		auto sphere = Sphere::CreateSphere(pShapeNode);
		_shapes.push_back(sphere);
		shapesPtr.push_back(cptr(sphere));
	}
	else if (shapeType == "Disk")
	{
		auto disk = crystal::Disk::CreateDisk(pShapeNode);
		_shapes.push_back(disk);
		shapesPtr.push_back(cptr(disk));
	}
	else if (shapeType == "TriangleMesh")
	{
		auto mesh = TriangleMesh::CreateTriangleMesh(pShapeNode);
		_triangleMeshes.push_back(mesh);
		auto triangles = mesh->GetTriangles();
		_shapes.insert(_shapes.end(), triangles.begin(), triangles.end());

		for (auto& triangle : triangles)
		{
			shapesPtr.push_back(cptr(triangle));
		}
	}
	else
	{
		throw std::invalid_argument("Invalid Shape Type!");
	}
	return shapesPtr;
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
	auto entity = std::make_shared<crystal::GeometricEntity>(shape, material, areaLight);
	_sceneObjects.push_back(entity);
}

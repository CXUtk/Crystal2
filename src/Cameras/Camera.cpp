#include "Camera.h"
#include <Cameras/PerspCamera.h>
#include <SJson/SJson.h>
#include <Loaders/JsonLoader.h>


std::unique_ptr<Camera> Camera::CreateCamera(JsonNode_CPTR pCameraNode, const config::ConfigInfo& configInfo) {
	auto cameraType = pCameraNode->GetMember("Type")->GetString();
	if (cameraType == "Normal") {
		auto lookAt = loader::parse_vec3(pCameraNode->GetMember("LookAt"));
		auto up = loader::parse_vec3(pCameraNode->GetMember("Up"));
		auto FOV = pCameraNode->GetMember("FOV")->GetFloat() / 180.f * glm::pi<float>();
		auto pos = loader::parse_vec3(pCameraNode->GetMember("Position"));
		return std::make_unique<PerspCamera>(pos, lookAt,
			up, FOV, (float)configInfo.Width / configInfo.Height, 1.0f, 100.0f);
	}
	else {
		throw std::invalid_argument("Invalid Camera Type!");
	}
}

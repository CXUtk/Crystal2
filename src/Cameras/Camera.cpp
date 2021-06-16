#include "Camera.h"
#include <Cameras/PerspCamera.h>
#include <SJson/SJson.h>
#include <Loaders/JsonLoader.h>


std::shared_ptr<Camera> Camera::CreateCamera(const std::shared_ptr<SJsonNode>& cameraNode, const config::ConfigInfo& configInfo) {
	auto cameraType = cameraNode->GetMember("Type")->GetString();
	if (cameraType == "Normal") {
		auto lookAt = loader::parse_vec3(cameraNode->GetMember("LookAt"));
		auto up = loader::parse_vec3(cameraNode->GetMember("Up"));
		auto FOV = cameraNode->GetMember("FOV")->GetFloat();
		auto pos = loader::parse_vec3(cameraNode->GetMember("Position"));
		return std::make_shared<PerspCamera>(pos, lookAt,
			up, FOV, (float)configInfo.Width / configInfo.Height, 1.0f, 100.0f);
	}
	else {
		throw std::invalid_argument("Invalid Camera Type!");
	}
}

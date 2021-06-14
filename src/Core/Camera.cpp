#include "Camera.h"
#include <Cameras/PerspCamera.h>
std::shared_ptr<Camera> Camera::CreateCamera(const scene::SceneInfo& sceneInfo) {
	switch (sceneInfo.Camera.Type) {
	case scene::CameraType::Normal:
		return std::make_shared<PerspCamera>(sceneInfo.Camera.Position, sceneInfo.Camera.LookAt,
			sceneInfo.Camera.Up, sceneInfo.Camera.FOV, (float)sceneInfo.Width / sceneInfo.Height, 1.0f, 100.0f);
	default:
	}
    return std::shared_ptr<Camera>();
}

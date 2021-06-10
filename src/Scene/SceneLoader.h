#pragma once
#include "SceneInfo.h"

namespace Scene {
    class SceneLoader {
    public:
        static SceneInfo LoadSceneInfo(const std::string& filePath);
    };
}
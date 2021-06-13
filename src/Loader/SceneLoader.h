#pragma once
#include "SceneInfo.h"

namespace scene {
    class SceneLoader {
    public:
        static SceneInfo LoadSceneInfo(const std::string& filePath);
    };
}
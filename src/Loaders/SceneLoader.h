#pragma once
#include "ConfigInfo.h"

namespace config {
    class SceneLoader {
    public:
        static SceneInfo LoadSceneInfo(const std::string& filePath);
    };
}
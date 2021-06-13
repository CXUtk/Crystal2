#pragma once
#include "SceneInfo.h"

namespace SceneLoader {
    class SceneLoader {
    public:
        static SceneInfo LoadSceneInfo(const std::string& filePath);
    };
}
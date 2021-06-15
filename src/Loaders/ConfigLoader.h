#pragma once
#include "ConfigInfo.h"

namespace config {
    class ConfigLoader {
    public:
        static ConfigInfo LoadConfigInfo(const std::string& filePath);
    };
}
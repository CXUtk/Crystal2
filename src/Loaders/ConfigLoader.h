#pragma once
#include <Crystal2.h>
#include <Core/ConfigInfo.h>
#include <memory>

namespace config {
    class ConfigLoader {
    public:
        static ConfigInfo LoadConfigInfo(const std::shared_ptr<SJsonNode>& nodes);
    };
}
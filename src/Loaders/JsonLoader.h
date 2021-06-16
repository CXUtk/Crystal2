#pragma once
#include <SJson/SJson.h>
#include <memory>
#include <glm/glm.hpp>

namespace loader {
    class JsonLoader {
    public:
        static std::shared_ptr<SJsonNode> LoadJsonFile(const std::string& filePath);
    };

    inline glm::vec3 parse_vec3(const std::shared_ptr<SJsonNode>& node) {
        assert(node->GetType() == SJsonNodeType::JSON_ARRAY);
        auto x = node->GetArrayIndex(0)->GetFloat();
        auto y = node->GetArrayIndex(1)->GetFloat();
        auto z = node->GetArrayIndex(2)->GetFloat();
        return glm::vec3(x, y, z);
    }
}
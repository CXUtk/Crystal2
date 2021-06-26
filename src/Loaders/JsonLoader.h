#pragma once
#include <SJson/SJson.h>
#include <memory>
#include <glm/glm.hpp>

namespace loader {
    class JsonLoader {
    public:
        static std::shared_ptr<SJson::SJsonNode> LoadJsonFile(const std::string& filePath);
    };

    inline glm::vec3 parse_vec3(const std::shared_ptr<SJson::SJsonNode>& node) {
        assert(node->GetType() == SJson::SJsonNodeType::JSON_ARRAY);
        auto x = node->ElementAt(0)->GetFloat();
        auto y = node->ElementAt(1)->GetFloat();
        auto z = node->ElementAt(2)->GetFloat();
        return glm::vec3(x, y, z);
    }
}
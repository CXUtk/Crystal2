#pragma once
#include <Crystal2.h>
#include <SJson/SJson.h>
#include <glm/glm.hpp>

namespace loader {
    class JsonLoader {
    public:
        static std::shared_ptr<SJson::SJsonNode> LoadJsonFile(const std::string& filePath);
    };

    inline glm::vec3 parse_vec3(JsonNode_CPTR pNode) {
        assert(pNode->GetType() == SJson::SJsonNodeType::JSON_ARRAY);
        auto x = pNode->ElementAt(0)->GetFloat();
        auto y = pNode->ElementAt(1)->GetFloat();
        auto z = pNode->ElementAt(2)->GetFloat();
        return glm::vec3(x, y, z);
    }
}
#include "ConfigLoader.h"
#include <SJson/SJson.h>
#include <iostream>
#include <fstream>
#include <sstream>

namespace config {

    ConfigInfo ConfigLoader::LoadConfigInfo(const std::shared_ptr<SJsonNode>& nodes) {
        assert(nodes->GetType() == SJsonNodeType::JSON_OBJECT);
        ConfigInfo info;
        info.Width = nodes->GetMember("Width")->GetInt();
        info.Height = nodes->GetMember("Height")->GetInt();
        info.SamplesPerPixel = nodes->GetMember("SamplesPerPixel")->GetInt();
        return info;
    }
}
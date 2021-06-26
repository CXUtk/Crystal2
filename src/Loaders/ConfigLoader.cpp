#include "ConfigLoader.h"
#include <SJson/SJson.h>
#include <iostream>
#include <fstream>
#include <sstream>

namespace config {

    ConfigInfo ConfigLoader::LoadConfigInfo(const std::shared_ptr<SJson::SJsonNode>& nodes) {
        assert(nodes->GetType() == SJson::SJsonNodeType::JSON_OBJECT);
        ConfigInfo info;
        info.Width = nodes->GetMember("Width")->GetInt();
        info.Height = nodes->GetMember("Height")->GetInt();
        info.SamplesPerPixel = nodes->GetMember("SamplesPerPixel")->GetInt();
        info.AccelType = nodes->GetMember("Acceleration")->GetString();
        return info;
    }
}
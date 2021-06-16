#include "JsonLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::shared_ptr<SJsonNode> loader::JsonLoader::LoadJsonFile(const std::string& filePath) {
    std::ifstream sceneFile;
    sceneFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    std::string sceneString;
    try {
        sceneFile.open(filePath);

        std::stringstream ssText;
        ssText << sceneFile.rdbuf();
        sceneString = ssText.str();
        sceneFile.close();
    }
    catch (std::ifstream::failure e) {
        std::cerr << "Failed to load scene!" << std::endl;
        throw std::exception(e.what());
    }

    SJsonErrorCode error;
    auto node = SJsonParse(sceneString, error);
    if (error != SJsonErrorCode::SJSON_OK) {
        fprintf(stderr, "Parse Scene File Failed! Error: %s\n", SJsonGetErrEnumName(error).c_str());
        throw;
    }
    return node;
}
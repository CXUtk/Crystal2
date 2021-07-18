#include "JsonLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::shared_ptr<SJson::SJsonNode> loader::JsonLoader::LoadJsonFile(const std::string& filePath) {
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
        fprintf(stderr, "Failed to load %s\n", filePath.c_str());
        throw std::exception(e.what());
    }

    std::shared_ptr<SJson::SJsonNode> node;
    try {
        node = SJson::SJsonParse(sceneString);
    }
    catch (const SJson::ExpectTokenError& e) {
        fprintf(stderr, "%s:%d:%d: %s. Expect token '%s', however got '%s'\n", filePath.c_str(), e.GetLine(), e.GetCol(), 
            e.what(), e.token.c_str(), e.curToken.c_str());
        throw;
    }
    catch (const SJson::ExpectValueError& e) {
        fprintf(stderr, "%s:%d:%d: %s. Expect value\n", filePath.c_str(), e.GetLine(), e.GetCol(),
            e.what());
        throw;
    }
    catch (const SJson::InvalidValueError& e) {
        fprintf(stderr, "%s:%d:%d: %s. Invalid value\n", filePath.c_str(), e.GetLine(), e.GetCol(),
            e.what());
        throw;
    }
    catch (const SJson::UnexpectedTokenError& e) {
        fprintf(stderr, "%s:%d:%d: %s. Unexpected token %s\n", filePath.c_str(), e.GetLine(), e.GetCol(),
            e.what(), e.token.c_str());
        throw;
    }
    catch (const SJson::RootNotSingularError& e) {
        fprintf(stderr, "%s:%d:%d: %s. Root not singluar\n", filePath.c_str(), e.GetLine(), e.GetCol(),
            e.what());
        throw;
    }
    catch (const std::exception& e) {
        fprintf(stderr, "%s: Error %s\n", filePath.c_str(), e.what());
        throw;
    }
    return node;
}

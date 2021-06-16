//#include "SceneLoader.h"
//#include <SJson/SJson.h>
//#include <iostream>
//#include <fstream>
//#include <sstream>
//
//
//namespace scene {
//
//    static void parse_integrator(SceneInfo& info, const std::shared_ptr<SJsonNode>& nodes) {
//        auto integratorTypeString = nodes->GetMember("Integrator")->GetString();
//        IntegratorType integratorType;
//        if (integratorTypeString == "PathTracer") {
//            integratorType = IntegratorType::PathTracer;
//        }
//        else if (integratorTypeString == "Whitted") {
//            integratorType = IntegratorType::WhittedStyle;
//        }
//        else if (integratorTypeString == "PRT") {
//            integratorType = IntegratorType::PRTTracer;
//        }
//        else {
//            fprintf(stderr, "Integrator Type Undefined!\n");
//            throw;
//        }
//        info.IntegratorType = integratorType;
//
//        info.SamplerType = nodes->GetMember("Sampler")->GetString();
//    }
//
//    static glm::vec3 parse_vec3(const std::shared_ptr<SJsonNode>& node) {
//        assert(node->GetType() == SJsonNodeType::JSON_ARRAY);
//        auto x = node->GetArrayIndex(0)->GetFloat();
//        auto y = node->GetArrayIndex(1)->GetFloat();
//        auto z = node->GetArrayIndex(2)->GetFloat();
//        return glm::vec3(x, y, z);
//    }
//
//    static void parse_camera(SceneInfo& info, const std::shared_ptr<SJsonNode>& nodes) {
//        auto& cameraNode = nodes->GetMember("Camera");
//        assert(cameraNode->GetType() == SJsonNodeType::JSON_OBJECT);
//
//
//        auto cameraTypeString = cameraNode->GetMember("Type")->GetString();
//        if (cameraTypeString == "Normal") {
//            info.Camera.Type = CameraType::Normal;
//            info.Camera.Position = parse_vec3(cameraNode->GetMember("Position"));
//            info.Camera.LookAt = parse_vec3(cameraNode->GetMember("LookAt"));
//            info.Camera.Up = parse_vec3(cameraNode->GetMember("Up"));
//            info.Camera.FOV = cameraNode->GetMember("FOV")->GetFloat();
//        }
//        else {
//            fprintf(stderr, "Camera Type Undefined!\n");
//            throw;
//        }
//    }
//
//    static void parse_objects(SceneInfo& info, const std::shared_ptr<SJsonNode>& nodes) {
//        auto objectsArray = nodes->GetMember("Objects");
//        assert(objectsArray->GetType() == SJsonNodeType::JSON_ARRAY);
//
//        std::vector<SceneObject> sceneObjects;
//
//        for (auto it = objectsArray->begin(); it != objectsArray->end(); it++) {
//            auto &node = (*it);
//            assert(node->GetType() == SJsonNodeType::JSON_OBJECT);
//            SceneObject object;
//            auto typeString = node->GetMember("Type")->GetString();
//            object.Type = typeString;
//            if (typeString == "Geometry") {
//
//            }
//            sceneObjects.push_back(object);
//        }
//        info.Objects = sceneObjects;
//    }
//
//
//    static SceneInfo parse_nodes(const std::shared_ptr<SJsonNode>& nodes) {
//        assert(nodes->GetType() == SJsonNodeType::JSON_OBJECT);
//        SceneInfo info;
//        info.Width = nodes->GetMember("Width")->GetInt();
//        info.Height = nodes->GetMember("Height")->GetInt();
//        info.SamplesPerPixel = nodes->GetMember("SamplesPerPixel")->GetInt();
//
//        parse_integrator(info, nodes);
//        parse_camera(info, nodes);
//        parse_objects(info, nodes);
//        return info;
//    }
//
//    SceneInfo SceneLoader::LoadSceneInfo(const std::string& filePath) {
//        std::ifstream sceneFile;
//        sceneFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
//
//        std::string sceneString;
//        try {
//            sceneFile.open(filePath);
//
//            std::stringstream ssText;
//            ssText << sceneFile.rdbuf();
//            sceneString = ssText.str();
//            sceneFile.close();
//        }
//        catch (std::ifstream::failure e) {
//            std::cerr << "Failed to load scene!" << std::endl;
//            throw std::exception(e.what());
//        }
//
//        SJsonErrorCode error;
//        auto node = SJsonParse(sceneString, error);
//        if (error != SJsonErrorCode::SJSON_OK) {
//            fprintf(stderr, "Parse Scene File Failed! Error: %s\n", SJsonGetErrEnumName(error).c_str());
//            throw;
//        }
//        return parse_nodes(node);
//    }
//}
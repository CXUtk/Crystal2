#include "Crystal2.h"
#include <Loaders/ConfigLoader.h>
#include <Loaders/SceneLoader.h>
#include <string>

int main(int argc, char** argv){
	if (argc < 3) {
		fprintf(stderr, "Usage: ./Crystal2 <config file> <scene file>\n");
		return 0;
	}
	else {
		auto configInfo = config::SceneLoader::LoadSceneInfo(std::string(argv[1]));
		fprintf(stdout, "Config Loaded!\n");

		auto sceneInfo = config::SceneLoader::LoadSceneInfo(std::string(argv[2]));
		fprintf(stdout, "Scene Loaded!\n");

		auto engine = Engine(configInfo, sceneInfo);
		engine.Run();
	}
	return 0;
}

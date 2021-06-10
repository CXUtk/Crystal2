#include "Crystal2.h"
#include "Scene/SceneLoader.h"
#include <string>

int main(int argc, char** argv){
	if (argc == 1) {
		fprintf(stderr, "Usage: ./Crystal2 <scene file>\n");
		return 0;
	}
	else {
		auto info = Scene::SceneLoader::LoadSceneInfo(std::string(argv[1]));
		fprintf(stdout, "Load Succeed!\n");
	}
	return 0;
}

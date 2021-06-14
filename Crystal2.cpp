#include "Crystal2.h"
#include "Loader/SceneLoader.h"
#include <string>

int main(int argc, char** argv){
	if (argc == 1) {
		fprintf(stderr, "Usage: ./Crystal2 <scene file>\n");
		return 0;
	}
	else {
		auto info = scene::SceneLoader::LoadSceneInfo(std::string(argv[1]));
		fprintf(stdout, "Scene Loaded!\n");

		auto engine = Engine(info);
		engine.Run();
	}
	return 0;
}

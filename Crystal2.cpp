﻿#include "Engine.h"
#include <Loaders/JsonLoader.h>
#include <cstdio>
#include <string>

int main(int argc, char** argv) {
	if (argc < 3) {
		fprintf(stderr, "Usage: ./Crystal2 <config file> <scene file>\n");
		return 0;
	}
	else {
		try {
			auto configNode = loader::JsonLoader::LoadJsonFile(std::string(argv[1]));
			fprintf(stdout, "Config Config File Loaded!\n");

			auto sceneNode = loader::JsonLoader::LoadJsonFile(std::string(argv[2]));
			fprintf(stdout, "Scene Config File Loaded!\n");

			auto engine = Engine(configNode, sceneNode);
			engine.Run();
		}
		catch (std::exception& e) {
			fprintf(stderr, "Terminated with error: %s\n", e.what());
		}
	}
	return 0;
}

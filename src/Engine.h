#pragma once
#include "Scene/SceneInfo.h"

class Engine {
public:
    Engine(const scene::SceneInfo& sceneInfo);
    ~Engine();

    void Run();
private:

};
#include "Integrator.h"
#include "WhittedIntegrator.h"
std::shared_ptr<Integrator> Integrator::LoadIntegrator(const scene::SceneInfo& sceneInfo)
{
	switch (sceneInfo.IntegratorType) 	{
	case scene::IntegratorType::PathTracer:
		break;
	case scene::IntegratorType::WhittedStyle:
		return std::make_shared<WhittedIntegrator>();
	case scene::IntegratorType::PRTTracer:
		break;
	default:
		break;
	}
}
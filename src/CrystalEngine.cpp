#include "CrystalEngine.h"
#include <Core/RayTracer.h>
#include <SJson/SJson.h>
#include <Loaders/ConfigLoader.h>
#include <Core/FrameBuffer.h>

#include <glm/gtx/transform.hpp>

#include <TREngine/Engine.h>
#include <TREngine/Core/Render/render.h>
#include <Core/Utils.h>

crystal::CrystalEngine::CrystalEngine(const std::shared_ptr<SJson::SJsonNode>& configNode, const std::shared_ptr<SJson::SJsonNode>& sceneInfo)
{
	try
	{
		_config = config::ConfigLoader::LoadConfigInfo(configNode);
		_rayTracer = std::make_shared<RayTracer>(_config, ::ptr(configNode), ::ptr(sceneInfo));
	}
	catch (const SJson::ConversionError& e)
	{
		fprintf(stderr, "%s: Cannot convert from %s to %s\n", e.what(), e.from.c_str(), e.to.c_str());
		throw;
	}
	catch (const SJson::MissingMemberError& e)
	{
		fprintf(stderr, "%s: Cannot find member named '%s'\n", e.what(), e.member.c_str());
		throw;
	}
	catch (const std::exception& e)
	{
		throw;
	}
}

crystal::CrystalEngine::~CrystalEngine()
{}


void crystal::CrystalEngine::Initialize(trv2::Engine* engine)
{
	fprintf(stdout, "All resources loaded, started ray tracing...\n");

	glm::vec3 x;
	bool s = refract(glm::vec3(-0.01, 0.999, -0.05), glm::vec3(-0.01, 0.999, -0.05), 0.688, &x);
	printf("%d\n", s);
	_rayTracer->TraceAsync();

	trv2::TextureParameters texPara{};
	_renderTexture = std::make_unique<trv2::Texture2D>(trv2::Engine::GetInstance()->GetGraphicsResourceManager(),
		glm::ivec2(_config.Width, _config.Height), texPara, trv2::PixelFormat::RGB, trv2::EngineDataType::UNSIGNED_BYTE, nullptr);
}

void crystal::CrystalEngine::Update(double deltaTime)
{}


void crystal::CrystalEngine::Draw(double deltaTime)
{
	auto renderer = trv2::Engine::GetInstance()->GetSpriteRenderer();
	auto graphicsDevice = trv2::Engine::GetInstance()->GetGraphicsDevice();
	auto universialRenderer = trv2::Engine::GetInstance()->GetUniversalRenderer();

	trv2::TextureParameters texPara{};
	_renderTexture->ChangeBuffer(glm::ivec2(_config.Width, _config.Height), texPara,
		trv2::PixelFormat::RGB, trv2::EngineDataType::UNSIGNED_BYTE, 
		_rayTracer->GetFrameBuffer()->GetImageDataRGB8(1 / 2.2).get());

	graphicsDevice->Clear(glm::vec4(0));

	auto transform = glm::ortho(0.f, (float)_config.Width,
	 0.f, (float)_config.Height);

	trv2::BatchSettings defaultSetting{};
	auto size = glm::vec2(_config.Width, _config.Height);
	renderer->Begin(transform, defaultSetting);
	{
		renderer->Draw(trv2::cptr(_renderTexture), glm::vec2(0), size, glm::vec2(0), 0.f, glm::vec4(1.f), trv2::SpriteFlipMode::FlipVertical);
	}
	renderer->End();

}

void crystal::CrystalEngine::Exit()
{}

bool crystal::CrystalEngine::ShouldSkipFrame(double elapsedFromFrameBegin)
{
	return false;
}

void crystal::CrystalEngine::OnFrameEnd()
{}

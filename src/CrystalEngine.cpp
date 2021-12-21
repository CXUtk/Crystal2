#include "CrystalEngine.h"
#include <Core/RayTracer.h>
#include <SJson/SJson.h>
#include <Loaders/ConfigLoader.h>
#include <Core/FrameBuffer.h>
#include <BSDFs/Lambertain.h>
#include <BSDFs/BlinnPhongReflection.h>
#include <BSDFs/Reflections.h>
#include <BSDFs/MicrofacetReflection.h>

#include <glm/gtx/transform.hpp>

#include <TREngine/Engine.h>
#include <TREngine/Core/Render/render.h>

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

	//_rayTracer->TraceAsync();

	trv2::TextureParameters texPara{};
	_renderTexture = std::make_unique<trv2::Texture2D>(trv2::Engine::GetInstance()->GetGraphicsResourceManager(),
		glm::ivec2(_config.Width, _config.Height), texPara, trv2::PixelFormat::RGB, trv2::EngineDataType::UNSIGNED_BYTE, nullptr);
}

void crystal::CrystalEngine::Update(double deltaTime)
{}


struct Data
{
	Vector3f unit;
	float v;
};

void crystal::CrystalEngine::Draw(double deltaTime)
{
	auto renderer = trv2::Engine::GetInstance()->GetSpriteRenderer();
	auto graphicsDevice = trv2::Engine::GetInstance()->GetGraphicsDevice();
	auto universialRenderer = trv2::Engine::GetInstance()->GetUniversalRenderer();

	trv2::TextureParameters texPara{};
	_renderTexture->ChangeBuffer(glm::ivec2(_config.Width, _config.Height), texPara,
		trv2::PixelFormat::RGB, trv2::EngineDataType::UNSIGNED_BYTE, _rayTracer->GetFrameBuffer()->GetImageData().get());

	graphicsDevice->Clear(glm::vec4(0));

	auto transform = glm::ortho((float)-_config.Width, (float)_config.Width,
		 (float)-_config.Height, (float)_config.Height);
	//auto transform = glm::ortho(0.f, (float)_config.Width,
	// 0.f, (float)_config.Height);

	//trv2::BatchSettings defaultSetting{};
	//auto size = glm::vec2(_config.Width, _config.Height);
	//renderer->Begin(transform, defaultSetting);
	//{
	//	renderer->Draw(trv2::cptr(_renderTexture), glm::vec2(0), size, glm::vec2(0), 0.f, glm::vec4(1.f), trv2::SpriteFlipMode::FlipVertical);
	//}
	//renderer->End();


	float root2 = std::sqrt(2) / 2.f;
	//Vector3f(-cos(0.1), sin(0.1), 0.f);
	Vector3f wo = Vector3f(-root2, root2, 0.f);


	auto fresnel = std::make_shared<FresnelDielectric>();
	auto beckmann = std::make_shared<BeckmannDistribution>(0.3f, 0.3f);
	auto bsdf = MicrofacetReflection(glm::vec3(1), 1.f, 1.5f, fresnel, beckmann);
	float radius = 500;
	std::vector<Data> dataList;

	float maxxV = 0.f;
	for (int i = 0; i < 1024; i++)
	{
		float r = i * glm::two_pi<float>() / 1024.f;
		Vector3f wi = Vector3f(glm::cos(r), glm::sin(r), 0.f);
		float vcosTheta = std::max(0.f, wi.y) * bsdf.DistributionFunction(wo, wi).r;
		dataList.push_back({ wi, vcosTheta });
		maxxV = std::max(maxxV, vcosTheta);
	}

	universialRenderer->DrawLine(Vector3f(-_config.Width, 0, 0), Vector3f(_config.Width, 0, 0), glm::vec4(1, 1, 0, 1), glm::vec4(1, 1, 0, 1));
	universialRenderer->DrawLine(Vector3f(0.f, -_config.Height, 0), Vector3f(0.f, _config.Height, 0), glm::vec4(1, 1, 0, 1), glm::vec4(1, 1, 0, 1));

	float scale = 1.f;
	if (maxxV > 1.f)
	{
		scale = 1.f / maxxV;
	}
	for (int i = 0; i < 1024; i++)
	{
		auto& cur = dataList[i];
		auto& nxt = dataList[(static_cast<std::vector<Data, std::allocator<Data>>::size_type>(i) + 1) % 1024];
		universialRenderer->DrawLine(cur.unit * cur.v * radius * scale, nxt.unit * nxt.v * radius * scale);
	}

	universialRenderer->DrawLine(wo * radius, Vector3f(0.f, 0, 0), glm::vec4(1, 0, 0, 1), glm::vec4(1, 0, 0, 1));

	universialRenderer->Flush(trv2::PrimitiveType::LINE_LIST, transform);
}

void crystal::CrystalEngine::Exit()
{}

bool crystal::CrystalEngine::ShouldSkipFrame(double elapsedFromFrameBegin)
{
	return false;
}

void crystal::CrystalEngine::OnFrameEnd()
{}

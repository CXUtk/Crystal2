#pragma once
#include <Crystal2.h>
#include <Core/ConfigInfo.h>
#include <TREngine/Application.h>

namespace crystal
{
	class CrystalEngine : public trv2::Application
	{
	public:
		CrystalEngine(const std::shared_ptr<SJson::SJsonNode>& configInfo, const std::shared_ptr<SJson::SJsonNode>& sceneInfo);
		~CrystalEngine() override;

		virtual void Initialize(trv2::Engine* engine) override;

		virtual void Update(double deltaTime) override;

		virtual void Draw(double deltaTime) override;

		virtual void Exit() override;

		virtual bool ShouldSkipFrame(double elapsedFromFrameBegin) override;

		virtual void OnFrameEnd() override;

	private:
		std::shared_ptr<RayTracer> _rayTracer;
		config::ConfigInfo _config;
		std::unique_ptr<trv2::Texture2D> _renderTexture;
		float _gamma = 2.2f;
	};
}
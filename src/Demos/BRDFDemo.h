#pragma once
#include <Crystal2.h>
#include <Core/ConfigInfo.h>
#include <TREngine/Application.h>

namespace crystal
{
	class BRDFDemo : public trv2::Application
	{
	public:
		BRDFDemo(const std::shared_ptr<SJson::SJsonNode>& configInfo);
		~BRDFDemo() override;

		virtual void Initialize(trv2::Engine* engine) override;

		virtual void Update(double deltaTime) override;

		virtual void Draw(double deltaTime) override;

		virtual void Exit() override;

		virtual bool ShouldSkipFrame(double elapsedFromFrameBegin) override;

		virtual void OnFrameEnd() override;

	private:
		config::ConfigInfo _config;
	};
}



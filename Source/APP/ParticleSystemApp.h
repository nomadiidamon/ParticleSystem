#pragma once
#include "AppData.h"
#include "Window.hpp"
#include "../precompiled.h"
#include "../Engine/Engine.h"
#include "../Engine/Renderer/UI/GUI.h"
#include "../Utils/IuvoUtils.h"

namespace APP {

	struct Input {
		GW::INPUT::GBufferedInput m_bufferedInput;
		GW::INPUT::GInput m_input;
		GW::INPUT::GController m_gamepad;
		GW::CORE::GEventCache m_pressEvents;
	};

	/// TODO: Add app to registry context
	class ParticleSystemApp {
		bool m_IsRunning = true;
		Input m_input;
		AppState m_state;
		AppUpdateStatus m_updateStatus;
		AppUpdateStatus m_fixedUpdateStatus;
		Engine::Engine m_engine;

	public:
		ParticleSystemApp(std::string appTitle, int xPos, int yPos, int width, int height, GWindowStyle windowMode);
		~ParticleSystemApp();
		void Run();
		void GraphicsBehavior(entt::registry& registry);
		void GameplayBehavior(entt::registry& registry);
		void MainLoopBehavior(entt::registry& registry);
		std::string levelPath, modelFolder;
		std::string vertShader, pixelShader, computeShader;
		Window m_appWindow;
		entt::entity appEntity;
	private:
		DRAW::RendererConfig CreateRendererInitData();
	};

}
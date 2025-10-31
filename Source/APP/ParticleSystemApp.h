#pragma once
#include "AppData.h"
#include "Window.hpp"
#include "../precompiled.h"

namespace APP {

	struct Input {
		GW::INPUT::GBufferedInput m_bufferedInput;
		GW::INPUT::GInput m_input;
		GW::INPUT::GController m_gamepad;
		GW::CORE::GEventCache m_pressEvents;
	};

	class ParticleSystemApp {
		XTime m_Time;
		float m_AccumulatedTime = 0.0f;
		bool m_IsRunning = true;
		Input m_input;
		Window m_appWindow;
		entt::registry* m_registry;
		AppState m_state;
		AppUpdateStatus m_updateStatus;
		AppUpdateStatus m_fixedUpdateStatus;

		/// add to appData?
		std::string levelPath, modelFolder;
		std::string vertShader, pixelShader;



		float m_FixedInterval = 0.02f; // 50 FPS
		float m_backgroundInterval = 0.1f; // 10 FPS

	public:
		ParticleSystemApp(std::string appTitle, int xPos, int yPos, int width, int height, GWindowStyle windowMode, entt::registry& _registry);
		~ParticleSystemApp();
		void Run();

		void GraphicsBehavior(entt::registry& registry);
		void GameplayBehavior(entt::registry& registry);
		void MainLoopBehavior(entt::registry& registry);
	private:
		void MainLoopIteration(entt::registry& registry);
		int Update(entt::registry& registry);
		int FixedUpdate(entt::registry& registry);

	};

}
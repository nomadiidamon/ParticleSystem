#pragma once
#include "Engine.h"
#include "../APP/Window.hpp"
#include "../Utils/UniversalComponents.h"


namespace Engine {


	Engine::Engine(entt::registry& _registry) : registry(_registry),
		m_renderer(DRAW::RendererConfig(), _registry)
	{
		m_Time = XTime(100, 0.9f); // 100 samples, 0.9 smooth factor
		m_Time.Restart();
	}

	Engine::~Engine()
	{
		Shutdown();
	}

	void Engine::Init()
	{
		if (m_isInitialized) {
			return;
		}

		auto logEnt = registry.view<UNIVERSAL::LogComponent>().front();
		auto& logger = registry.get<UNIVERSAL::LogComponent>(logEnt);
		logger.Log("Engine --> Initializing Engine...");

		m_renderer.InitializeRenderer();

		logger.GreenLog("Engine Initialization Completed successfully.");
		m_isInitialized = true;
	}

	int Engine::UpdateEngine() {
		int status_U = Update(registry);
		int status_FU = FixedUpdate(registry);
		m_Time.Signal();

		if (status_U == -1 || status_FU == -1) {
			auto logEnt = registry.view<UNIVERSAL::LogComponent>().front();
			auto& logger = registry.get<UNIVERSAL::LogComponent>(logEnt);
			if (status_U == -1) {
				logger.RedLog("Engine::Update() requested shutdown.");
			}
			if (status_FU == -1) {
				logger.RedLog("Engine::FixedUpdate() requested shutdown.");
			}
			m_IsRunning = false;
			return -1; // exit code
		}
		return 0;
	}

	void Engine::Shutdown()
	{
		UNIVERSAL::LogComponent logger;
		logger.InitLogger(false, false);
		/// TODO: Shutdown Renderer and other systems
		m_renderer.ShutdownRenderer(registry, logger);


		logger.RedLog("Engine: Shutting Down Engine...");
	}


	int Engine::Update(entt::registry& registry) {
		/// TODO: Update Renderer and other systems
		m_renderer.UpdateRenderer(registry);

		// Check for exit condition
		if (m_IsRunning == false) {
			return -1; // exit code
		}

		return 0;
	}

	int Engine::FixedUpdate(entt::registry& registry) {
		float deltaTime = (float)m_Time.SmoothDelta();
		m_AccumulatedTime += deltaTime;
		while (m_AccumulatedTime >= m_FixedInterval) {
			/// TODO: Fixed Update Logic for Physics, AI, etc.

			m_AccumulatedTime -= m_FixedInterval;
		}

		m_AccumulatedTime = fmod(m_AccumulatedTime, m_FixedInterval);

		// Check for exit condition
		if (m_IsRunning == false) {
			return -1; // exit code
		}

		return 0;
	}


}// namespace UNIVERSAL
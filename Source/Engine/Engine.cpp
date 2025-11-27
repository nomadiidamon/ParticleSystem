#pragma once
#include "Engine.h"
#include "../APP/Window.hpp"


namespace Engine {


	Engine::Engine() :
		m_renderer(DRAW::RendererConfig())
	{
		m_Time = XTime(100, 0.9f); // 100 samples, 0.9 smooth factor
		m_Time.Restart();
	}

	Engine::~Engine()
	{
		Shutdown();
	}

	int Engine::UpdateEngine() {
		int status_U = Update(registry);
		int status_FU = FixedUpdate(registry);
		m_Time.Signal();
		if (status_U == -1 && status_FU == -1) {
			m_IsRunning = false;
			return -1; // exit code
		}
		return 0;
	}

	void Engine::Init()
	{
		if (m_isInitialized) {
			return;
		}

		m_renderer.InitializeRenderer();

		std::cout << "Engine Initialization Logic Executed." << std::endl;
		m_isInitialized = true;
	}

	void Engine::Shutdown()
	{
		std::cout << "Shutting down Engine..." << std::endl;
		/// TODO: Shutdown Renderer and other systems
	
	}

	int Engine::Update(entt::registry& registry) {
		if (m_IsRunning == false) {
			/// TODO: Updated Renderer and other systems

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

		if (m_IsRunning == false) {
			return -1; // exit code
		}
		return 0;
	}


}// namespace UNIVERSAL
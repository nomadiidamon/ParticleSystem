#include "ParticleSystemApp.h"

namespace APP {

	ParticleSystemApp::ParticleSystemApp(std::string appTitle, int xPos, int yPos, int width, int height, GWindowStyle windowMode, entt::registry& _registry) {

		std::cout << "Booting Up Particle System!" << std::endl;

		// Store the registry reference
		m_registry = &_registry;

		// Initialize time
		m_Time = XTime(100, 0.9f); // 100 samples, 0.9 smooth factor
		m_Time.Restart();

		// Set up the application window
		m_appWindow.title = appTitle;
		m_appWindow.x = xPos;
		m_appWindow.y = yPos;
		m_appWindow.width = width;
		m_appWindow.height = height;
		m_appWindow.style = windowMode;

		// Create the application window entity
		auto windowEntity = m_registry->create();
		m_registry->emplace<Window>(windowEntity, m_appWindow);
		auto& windowComp = m_registry->get<GW::SYSTEM::GWindow>(windowEntity);
		if (!windowComp) {
			std::cerr << "Failed to create application window!" << std::endl;
			m_IsRunning = false;
		}
		m_input.m_input.Create(windowComp);
		m_input.m_bufferedInput.Create(windowComp);
		m_input.m_gamepad.Create();
		m_input.m_pressEvents.Create(32);
		m_input.m_bufferedInput.Register(m_input.m_pressEvents);
	}

	ParticleSystemApp::~ParticleSystemApp() {

	}

	void ParticleSystemApp::Run() {

		GraphicsBehavior(*m_registry); // create windows, surfaces, and renderers
		GameplayBehavior(*m_registry); // create entities and components for gameplay
		MainLoopBehavior(*m_registry); // update windows and input
	}

	// Set up graphics entities and components
	void ParticleSystemApp::GraphicsBehavior(entt::registry& registry) {
		std::cout << "Graphics Initialized!" << std::endl;
	}

	/// Set up gameplay entities and components
	void ParticleSystemApp::GameplayBehavior(entt::registry& registry) {
		std::cout << "Gameplay Initialized!" << std::endl;
	}

	// Main loop behavior: update windows and input
	void ParticleSystemApp::MainLoopBehavior(entt::registry& registry) {
		std::cout << "MainLoop Initialized!" << std::endl;

		do 
		{ 
			MainLoopIteration(registry); 
		} 
		while (m_IsRunning);

		std::cout << "Shutting Down Particle System!" << std::endl;
	}

	int ParticleSystemApp::Update(entt::registry& registry) {
		//std::cout << "Running Base Update" << std::endl;
		if (m_IsRunning == false) {
			return -1; // exit code
		}
		return 0;
	}

	int ParticleSystemApp::FixedUpdate(entt::registry& registry) {
		//std::cout << "Running Fixed Update" << std::endl;

		// get delta time
		float deltaTime = (float)m_Time.SmoothDelta();
		// accumulate time
		m_AccumulatedTime += deltaTime;
		// if enough time has passed, run fixed update

		int iterations = 0;
		while (m_AccumulatedTime >= m_FixedInterval) {
			//std::cout << "Performing Fixed Update Logic" << std::endl;
			iterations++;
			// perform fixed update logic here



			// reduce accumulated time
			m_AccumulatedTime -= m_FixedInterval;
		}
		m_AccumulatedTime = fmod(m_AccumulatedTime, m_FixedInterval);
		//std::cout << "Last Iteration Count: " << iterations <<  std::endl;

		if (m_IsRunning == false) {
			return -1; // exit code
		}
		return 0;
	}

	void ParticleSystemApp::MainLoopIteration(entt::registry& registry) {
		int closedCount = 0; // count of closed windows
		auto winView = registry.view<APP::Window>(); // for updating all windows
		// find all Windows that are not closed and call "patch" to update them
		for (auto entity : winView) {
			if (registry.any_of<APP::WindowClosed>(entity))
				++closedCount;
			else
				registry.patch<APP::Window>(entity); // calls on_update()
		}

		float status_X = 0.0f;
		m_input.m_input.GetState(G_KEY_X, status_X);
		if (status_X > 0.0f) {
			m_IsRunning = false;
		}

		int status_U = Update(registry);
		int status_FU = FixedUpdate(registry);
		m_Time.Signal();
		if (status_U == -1 && status_FU == -1) {
			m_IsRunning = false;
		}
	}

}
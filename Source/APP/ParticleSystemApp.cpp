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

		// Set up level and shader paths
		levelPath = "../Assets/Levels/Test.json";
		modelFolder = "../Assets/Levels/Models/";
		vertShader = "../Shaders/ModelBased/LevelVertex.hlsl";
		pixelShader = "../Shaders/ModelBased/LevelPixel.hlsl";

		entt::entity appEntity = m_registry->create();
		m_registry->emplace<APP::ParticleSystemApp>(appEntity, *this);
		m_registry->emplace<Engine::Engine>(appEntity, Engine::Engine(*m_registry));
		m_registry->emplace<Engine::UI::GUIManager>(appEntity, Engine::UI::GUIManager(*m_registry));
		DRAW::VulkanRendererInitialization initData{
			vertShader, pixelShader,
			{ {0.2f, 0.2f, 0.25f, 1} } , { 1.0f, 0u }, 75.f, 0.1f, 100.0f
		};
		m_registry->emplace<Engine::Renderer>(appEntity, Engine::Renderer(*m_registry, initData));
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

		// Create the main entity for the display
		auto display = registry.create();

		// Create the CPU level component
		registry.emplace<DRAW::CPULevel>(display, DRAW::CPULevel{ levelPath, modelFolder });

		// Create the application window component
		registry.emplace<Window>(display, m_appWindow);
		auto& windowComp = m_registry->get<GW::SYSTEM::GWindow>(display);
		if (!windowComp) {
			std::cerr << "Failed to create application window!" << std::endl;
			m_IsRunning = false;
		}
		m_input.m_input.Create(windowComp);
		m_input.m_bufferedInput.Create(windowComp);
		m_input.m_gamepad.Create();
		m_input.m_pressEvents.Create(32);
		m_input.m_bufferedInput.Register(m_input.m_pressEvents);


		// Create a transient component to initialize the Renderer
		registry.emplace<DRAW::VulkanRendererInitialization>(display,
			DRAW::VulkanRendererInitialization{
				vertShader, pixelShader,
				{ {0.2f, 0.2f, 0.25f, 1} } , { 1.0f, 0u }, 75.f, 0.1f, 100.0f });
		registry.emplace<DRAW::VulkanRenderer>(display);

		// TODO : Emplace GPULevel
		registry.emplace<DRAW::GPULevel>(display);

		// Register for Vulkan clean up
		GW::CORE::GEventResponder shutdown;
		shutdown.Create([&](const GW::GEvent& e) {
			GW::GRAPHICS::GVulkanSurface::Events event;
			GW::GRAPHICS::GVulkanSurface::EVENT_DATA data;
			if (+e.Read(event, data) && event == GW::GRAPHICS::GVulkanSurface::Events::RELEASE_RESOURCES) {
				registry.clear<DRAW::VulkanRenderer>();
			}
			});
		registry.get<DRAW::VulkanRenderer>(display).vlkSurface.Register(shutdown);
		registry.emplace<GW::CORE::GEventResponder>(display, shutdown.Relinquish());


		// Create a camera and emplace it
		GW::MATH::GMATRIXF initialCamera;
		GW::MATH::GVECTORF translate = { 0.0f,  45.0f, -5.0f };
		GW::MATH::GVECTORF lookat = { 0.0f, 0.0f, 0.0f };
		GW::MATH::GVECTORF up = { 0.0f, 1.0f, 0.0f };
		GW::MATH::GMatrix::TranslateGlobalF(initialCamera, translate, initialCamera);
		GW::MATH::GMatrix::LookAtLHF(translate, lookat, up, initialCamera);
		// Inverse to turn it into a camera matrix, not a view matrix. This will let us do
		// camera manipulation in the component easier
		GW::MATH::GMatrix::InverseF(initialCamera, initialCamera);
		registry.emplace<DRAW::CameraComponent>(display,
			DRAW::CameraComponent{ initialCamera });
	}

	/// Set up gameplay entities and components
	void ParticleSystemApp::GameplayBehavior(entt::registry& registry) {
		std::cout << "Gameplay Initialized!" << std::endl;
	}

	// Main loop behavior: update windows and input
	void ParticleSystemApp::MainLoopBehavior(entt::registry& registry) {
		std::cout << "MainLoop Initialized!" << std::endl;

		int closedCount = 0; // count of closed windows
		auto winView = registry.view<APP::Window>(); // for updating all windows
		do
		{
			MainLoopIteration(registry);

			// update Engine component
			auto& engines = m_registry->view<Engine::Engine>();
			if (engines.size_hint() != (size_t)0) {
				for (auto& engine : engines) {
					m_registry->patch<Engine::Engine>(engine);
				}
			}

			// find all Windows that are not closed and call "patch" to update them
			for (auto entity : winView) {
				if (registry.any_of<APP::WindowClosed>(entity))
					++closedCount;
				else
					registry.patch<APP::Window>(entity); // calls on_update()
			}
		} while (winView.size() != closedCount);

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
		// Check for exit input (Escape key or X button on window)
		float status_X = 0.0f;
		m_input.m_input.GetState(G_KEY_X, status_X);
		if (status_X > 0.0f) {
			m_IsRunning = false;
			auto& winView = registry.view<APP::Window>();
			winView.each([&](auto entity, APP::Window& windowComp) {
				registry.emplace<APP::WindowClosed>(entity);
				});
		}
		int status_U = Update(registry);
		int status_FU = FixedUpdate(registry);
		m_Time.Signal();
		if (status_U == -1 && status_FU == -1) {
			m_IsRunning = false;
		}
	}

}
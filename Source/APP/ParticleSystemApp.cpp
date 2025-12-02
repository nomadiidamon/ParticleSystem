#include "ParticleSystemApp.h"

namespace APP {

	ParticleSystemApp::ParticleSystemApp(std::string appTitle, int xPos, int yPos, int width, int height, GWindowStyle windowMode, entt::registry& _registry) {

		std::cout << "Booting Up Particle System!" << std::endl;

		// Store the registry reference
		m_registry = &_registry;


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

		// Initialize time
		m_Time = XTime(100, 0.9f); // 100 samples, 0.9 smooth factor
		m_Time.Restart();

		// Create the main application entity and emplace core components
		entt::entity appEntity = m_registry->create();
		m_registry->emplace<APP::ParticleSystemApp>(appEntity, *this);
	}

	ParticleSystemApp::~ParticleSystemApp() {

	}

	void ParticleSystemApp::Run() {
		GraphicsBehavior(*m_registry); // create windows, surfaces, and renderers
		GameplayBehavior(*m_registry); // create entities and components for gameplay
		MainLoopBehavior(*m_registry); // update windows and input
	}

	XTime& ParticleSystemApp::GetTime()
	{
		return m_Time;
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

		// create a secondary test window for ImGui
		entt::entity imguiWindow = registry.create();
		APP::Window_Vk_ImGui imguiWinComp;
		imguiWinComp.title = "ImGui Window";
		imguiWinComp.x = m_appWindow.x + m_appWindow.width / 2;
		imguiWinComp.y = m_appWindow.y;
		imguiWinComp.width = 400;
		imguiWinComp.height = 400;
		imguiWinComp.style = GW::SYSTEM::GWindowStyle::WINDOWEDBORDERED;
		registry.emplace<Window_Vk_ImGui>(imguiWindow, imguiWinComp);
		auto& imguiWindowComp = m_registry->get<GW::SYSTEM::GWindow>(imguiWindow);
		if (!imguiWindowComp) {
			std::cerr << "Failed to create ImGui window!" << std::endl;
			m_IsRunning = false;
		}


		//// create a window with vulkan surface component for the main display
		//entt::entity vwvsEntity = registry.create();
		//WindowWithVulkanSurface vwvsComp{
		//	m_appWindow.x + 75,
		//	m_appWindow.y + 250,
		//	400,
		//	400,
		//	m_appWindow.style,
		//	m_appWindow.title + " - Vulkan Surface"
		//};
		//registry.emplace<WindowWithVulkanSurface>(vwvsEntity, vwvsComp);
		//auto& vulkanWindowComp = m_registry->get<APP::WindowWithVulkanSurface>(vwvsEntity);
		//if (!vulkanWindowComp.GetWindow()) {
		//	std::cerr << "Failed to create WindowWithVulkanSurface!" << std::endl;
		//	m_IsRunning = false;
		//}

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

		// emplace a vulkan renderer for the ImGui window as well
		registry.emplace<DRAW::VulkanRendererInitialization>(imguiWindow,
			DRAW::VulkanRendererInitialization{
				vertShader, pixelShader,
				{ {0.45f, 0.35f, 0.85f, 1} } , { 1.0f, 0u }, 75.f, 0.1f, 100.0f });
		registry.emplace<DRAW::VulkanRenderer>(imguiWindow);

		//// emplace a vulkan initialization for the WindowWithVulkanSurface
		//registry.emplace<DRAW::VulkanRendererInitialization>(vwvsEntity,
		//	DRAW::VulkanRendererInitialization{
		//		vertShader, pixelShader,
		//		{ {0.8f, 0.15f, 0.15f, 1} } , { 1.0f, 0u }, 75.f, 0.1f, 100.0f });
		//vwvsComp.vulkanRenderer = registry.emplace<DRAW::VulkanRenderer>(vwvsEntity);
		//


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
		registry.get<DRAW::VulkanRenderer>(imguiWindow).vlkSurface.Register(shutdown);
		registry.emplace<GW::CORE::GEventResponder>(imguiWindow, shutdown.Relinquish());
		//vwvsComp.GetSurface().Register(shutdown);
		//registry.emplace<GW::CORE::GEventResponder>(vwvsEntity, shutdown.Relinquish());
	}

	/// Set up gameplay entities and components
	void ParticleSystemApp::GameplayBehavior(entt::registry& registry) {
		std::cout << "Gameplay Initialized!" << std::endl;
		auto appEntity = registry.view<APP::ParticleSystemApp>().front();
		m_registry->emplace<Engine::Engine>(appEntity, Engine::Engine(*m_registry));
	}

	// Main loop behavior: update windows and input
	void ParticleSystemApp::MainLoopBehavior(entt::registry& registry) {
		std::cout << "MainLoop Initialized!" << std::endl;

		int winClosedCount = 0; // count of closed windows
		int guiClosedCount = 0; // count of closed ImGui windows
		auto winView = registry.view<APP::Window>(); // for updating all windows
		auto winGUIView = registry.view<APP::Window_Vk_ImGui>(); // for updating all ImGui windows
		auto vwvsView = registry.view<APP::WindowWithVulkanSurface>(); // for updating all WindowWithVulkanSurface
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
					++winClosedCount;
				else
					registry.patch<APP::Window>(entity); // calls on_update()
			}
			for (auto entity : winGUIView) {
				if (registry.any_of<APP::WindowClosed>(entity))
					++guiClosedCount;
				else
					registry.patch<APP::Window_Vk_ImGui>(entity); // calls on_update()
			}
			for (auto entity : vwvsView) {
				if (!registry.any_of<APP::WindowClosed>(entity))
					registry.patch<APP::WindowWithVulkanSurface>(entity); // calls on_update()
			}
			static int testInt = 0;
			if (testInt == 150) {
				std::cout << "Window Count: " << winView.size() << std::endl;
				std::cout << "Window Closed Count: " << winClosedCount << std::endl;
				std::cout << "ImGui Window Count: " << winGUIView.size() << std::endl;
				std::cout << "ImGui Window Closed Count: " << guiClosedCount << std::endl;
				testInt = 0;
			}
			else {
				testInt++;
			}

		} while ((winClosedCount != winView.size()));

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

		int status_U = Update(registry);
		int status_FU = FixedUpdate(registry);
		m_Time.Signal();
		if (status_U == -1 && status_FU == -1) {
			m_IsRunning = false;
		}
	}

}
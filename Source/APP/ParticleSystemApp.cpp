#include "ParticleSystemApp.h"

namespace APP {

	ParticleSystemApp::ParticleSystemApp(std::string appTitle, int xPos, int yPos, int width, int height, GWindowStyle windowMode) {

		std::cout << "Booting Up Particle System!" << std::endl;

		m_appWindow.title = appTitle;
		m_appWindow.x = xPos;
		m_appWindow.y = yPos;
		m_appWindow.width = width;
		m_appWindow.height = height;
		m_appWindow.style = windowMode;

		auto logEntity = registry.create();
		registry.emplace<UNIVERSAL::LogComponent>(logEntity);
	}

	ParticleSystemApp::~ParticleSystemApp() {

	}

	void ParticleSystemApp::Run() {
		GraphicsBehavior(registry); // create windows, surfaces, and renderers
		GameplayBehavior(registry); // create entities and components for gameplay
		MainLoopBehavior(registry); // update windows and input
	}

	void ParticleSystemApp::GraphicsBehavior(entt::registry& registry) {
		std::cout << "Graphics Initialized!" << std::endl;

		// Create the application window component
		appEntity = registry.create();
		auto& window = registry.emplace<Window>(appEntity, m_appWindow);

		// Create the renderer configuration component
		DRAW::RendererConfig renderInitData = CreateRendererInitData();
		registry.emplace<DRAW::RendererConfig>(appEntity, renderInitData);

		// Register for Vulkan clean up
		GW::CORE::GEventResponder shutdown;
		shutdown.Create([&](const GW::GEvent& e) {
			GW::GRAPHICS::GVulkanSurface::Events event;
			GW::GRAPHICS::GVulkanSurface::EVENT_DATA data;
			if (+e.Read(event, data) && event == GW::GRAPHICS::GVulkanSurface::Events::RELEASE_RESOURCES) {
				registry.clear<Engine::Renderer>();
			}
			});
		m_engine.m_renderer.GetVulkanRenderer().vlkSurface.Register(shutdown);
		registry.emplace<GW::CORE::GEventResponder>(appEntity, shutdown.Relinquish());

		// finally, initialize the engine
		m_engine.Init();
	}

	/// Set up gameplay entities and components
	void ParticleSystemApp::GameplayBehavior(entt::registry& registry) {
		std::cout << "Gameplay Initialized!" << std::endl;

	}

	// Main loop behavior: update windows and input
	void ParticleSystemApp::MainLoopBehavior(entt::registry& registry) {
		std::cout << "MainLoop Initialized!" << std::endl;

		int winClosedCount = 0; // count of closed windows
		auto winView = registry.view<APP::Window>();
		int engineStatus = 0;
		do
		{
			engineStatus = m_engine.UpdateEngine();


			// find all Windows that are not closed and call "patch" to update them
			for (auto entity : winView) {
				if (registry.any_of<APP::WindowClosed>(entity))
					++winClosedCount;
				else
					registry.patch<APP::Window>(entity); // calls on_update()
			}

		} while ((winClosedCount != winView.size()) || (engineStatus != 0));

		std::cout << "Shutting Down Particle System!" << std::endl;
	}


	DRAW::RendererConfig ParticleSystemApp::CreateRendererInitData()
	{
		DRAW::RendererConfig renderInitData =
		{
			vertShader,
			pixelShader,
			computeShader,
			{ {0.2f, 0.2f, 0.25f, 1} }, // clear color
			{ 1.0f, 0u }, // depth stencil
			75.f, // fov degrees
			0.1f, // near plane
			1000.0f // far plane
		};
		return renderInitData;
	}

}
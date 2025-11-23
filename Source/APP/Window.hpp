#include "Engine/Renderer/DRAW/DrawComponents.h"
#include "../precompiled.h"

namespace APP
{
	//*** TAGS ***//
	struct WindowClosed {};

	/// TODO: Create a second window that integrates with ImGui automatically
	struct Window_Vk_ImGui {
		int x = 0, y = 0, width = 100, height = 100;
		GW::SYSTEM::GWindowStyle style = GW::SYSTEM::GWindowStyle::WINDOWEDBORDERED;
		std::string title = "Unitialized ImGui Window";	
	};


	/// TODO: Create a pure ImGui window, no platform window



	//*** COMPONENTS ***//
	struct Window
	{
		int x = 0, y = 0, width = 100, height = 100;
		GW::SYSTEM::GWindowStyle style = GW::SYSTEM::GWindowStyle::WINDOWEDBORDERED;
		std::string title = "Unitialized";
	};

	//*** SYSTEMS ***//
	// run this code when a Window component is created
	inline void Construct_Window(entt::registry& registry, entt::entity entity) {

		// Create the window to render in
		auto& window = registry.get<Window>(entity);
		GW::SYSTEM::GWindow newWindow;
		if (+newWindow.Create(window.x, window.y, window.width, window.height, window.style)) {
			newWindow.SetWindowName(window.title.c_str());
			registry.emplace<GW::SYSTEM::GWindow>(entity, newWindow.Relinquish());
		}
		else
			std::cout << "Failed to create window" << std::endl;
	}

	inline void Construct_Window_Vk_ImGui(entt::registry& registry, entt::entity entity) {
		// Create the window to render in
		auto& window = registry.get<Window_Vk_ImGui>(entity);
		GW::SYSTEM::GWindow newWindow;
		if (+newWindow.Create(window.x, window.y, window.width, window.height, window.style)) {
			newWindow.SetWindowName(window.title.c_str());
			registry.emplace<GW::SYSTEM::GWindow>(entity, newWindow.Relinquish());
		}
		else
			std::cout << "Failed to create ImGui window" << std::endl;		
	}

	// run this code when a Window component is updated
	inline void Update_Window(entt::registry& registry, entt::entity entity) {

		// If we get a close window event, emplace a WindowClosed component
		auto& win = registry.get<GW::SYSTEM::GWindow>(entity);
		if (-win.ProcessWindowEvents()) {
			registry.emplace_or_replace<WindowClosed>(entity);
		}
		// update any 3D graphics surfaces that are attached to this window
		else if (registry.any_of<DRAW::VulkanRenderer>(entity)) {
			// update the window's GVulkanSurface if it has one
			registry.patch<DRAW::VulkanRenderer>(entity);
		}
		
	}

	inline void Update_Window_Vk_ImGui(entt::registry& registry, entt::entity entity) {
		// If we get a close window event, emplace a WindowClosed component
		auto& win = registry.get<GW::SYSTEM::GWindow>(entity);
		if (-win.ProcessWindowEvents()) {
			registry.emplace_or_replace<WindowClosed>(entity);
		}
		// update any ImGui surfaces that are attached to this window
		else if (registry.any_of<DRAW::VulkanRenderer>(entity)) {
			// update the window's GVulkanSurface if it has one
			registry.patch<DRAW::VulkanRenderer>(entity);
		}
	}

	inline void Destroy_Window(entt::registry& registry, entt::entity entity) {
		// Clean
		if (registry.any_of<GW::SYSTEM::GWindow>(entity)) {
			auto& win = registry.get<GW::SYSTEM::GWindow>(entity);
			win.Relinquish();
			registry.remove<GW::SYSTEM::GWindow>(entity);
		}
	}

	inline void Destroy_Window_Vk_ImGui(entt::registry& registry, entt::entity entity) {
		// Clean
		if (registry.any_of<GW::SYSTEM::GWindow>(entity)) {
			auto& win = registry.get<GW::SYSTEM::GWindow>(entity);
			win.Relinquish();
			registry.remove<GW::SYSTEM::GWindow>(entity);
		}
	}

	// Use this MACRO to connect the EnTT Component Logic
	CONNECT_COMPONENT_LOGIC() {
		// register the Window component's logic
		registry.on_construct<Window>().connect<Construct_Window>();
		registry.on_update<Window>().connect<Update_Window>();
		registry.on_destroy<Window>().connect<Destroy_Window>();

		registry.on_construct<Window_Vk_ImGui>().connect<Construct_Window_Vk_ImGui>();
		registry.on_update<Window_Vk_ImGui>().connect<Update_Window_Vk_ImGui>();
		registry.on_destroy<Window_Vk_ImGui>().connect<Destroy_Window_Vk_ImGui>();

	}
} // namespace APP
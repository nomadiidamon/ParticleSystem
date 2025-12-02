#pragma once
#include "Engine/Renderer/DRAW/DrawComponents.h"
#include "../precompiled.h"

namespace APP
{
	//*** COMPONENTS ***//
	struct WindowWithVulkanSurface
	{
		// Window properties
		int x = 100, y = 600, width = 300, height = 300;
		GW::SYSTEM::GWindowStyle style = GW::SYSTEM::GWindowStyle::WINDOWEDBORDERED;
		std::string title = "Uninitialized WindowWithVulkanSurface";

		// Vulkan surface and device handles
		GW::SYSTEM::GWindow window;
		DRAW::VulkanRenderer vulkanRenderer;

		// expose device, surface, etc. for external use
		VkDevice GetDevice() const { return vulkanRenderer.device; }
		GW::GRAPHICS::GVulkanSurface& GetSurface() { return vulkanRenderer.vlkSurface; }
		GW::SYSTEM::GWindow& GetWindow() { return window; }
	};

	//*** SYSTEMS ***//
	inline void Construct_WindowWithVulkanSurface(entt::registry& registry, entt::entity entity)
	{
		auto& wwvs = registry.get<WindowWithVulkanSurface>(entity);

		// Create the window
		if (+wwvs.window.Create(wwvs.x, wwvs.y, wwvs.width, wwvs.height, wwvs.style)) {
			wwvs.window.SetWindowName(wwvs.title.c_str());
			std::cout << "Window created!" << std::endl;
			registry.emplace<GW::SYSTEM::GWindow>(entity, wwvs.window.Relinquish());
		}
		else {
			std::cout << "Failed to create WindowWithVulkanSurface window" << std::endl;
			return;
		}

		// Create the Vulkan surface
#ifndef NDEBUG
		const char* debugLayers[] = { "VK_LAYER_KHRONOS_validation" };
		if (-wwvs.vulkanRenderer.vlkSurface.Create(wwvs.window, GW::GRAPHICS::DEPTH_BUFFER_SUPPORT,
			sizeof(debugLayers) / sizeof(debugLayers[0]), debugLayers, 0, nullptr, 0, nullptr, false))
#else
		if (-wwvs.vulkanRenderer.vlkSurface.Create(wwvs.window, GW::GRAPHICS::DEPTH_BUFFER_SUPPORT))
#endif
		{
			std::cout << "Failed to create Vulkan Surface for WindowWithVulkanSurface!" << std::endl;
			return;
		}
		std::cout << "Surface created!!" << std::endl;


	}

	inline void Update_WindowWithVulkanSurface(entt::registry& registry, entt::entity entity)
	{
		auto& wwvs = registry.get<WindowWithVulkanSurface>(entity);
		if (-wwvs.window.ProcessWindowEvents()) {
			registry.emplace_or_replace<WindowClosed>(entity);
		}
		else if (registry.any_of<DRAW::VulkanRenderer>(entity)) {
			registry.patch<DRAW::VulkanRenderer>(entity);
		}
	}


	inline void Destroy_WindowWithVulkanSurface(entt::registry& registry, entt::entity entity)
	{
		auto& wwvs = registry.get<WindowWithVulkanSurface>(entity);
		// Wait for device idle if needed
		if (wwvs.vulkanRenderer.device)
			vkDeviceWaitIdle(wwvs.vulkanRenderer.device);

		// Destroy Vulkan surface and window
		wwvs.vulkanRenderer.vlkSurface = {};
		wwvs.window.Relinquish();
	}

	// Use this MACRO to connect the EnTT Component Logic
	CONNECT_COMPONENT_LOGIC() {
		registry.on_construct<WindowWithVulkanSurface>().connect<Construct_WindowWithVulkanSurface>();
		registry.on_update<WindowWithVulkanSurface>().connect<Update_WindowWithVulkanSurface>();
		registry.on_destroy<WindowWithVulkanSurface>().connect<Destroy_WindowWithVulkanSurface>();
	}
} // namespace APP

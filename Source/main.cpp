#include <iostream>
#include "precompiled.h"
#include "App/App.h"

int AppCycle(GWindow &_win)
{
	GWindow win;
	GEventResponder msgs;
	GVulkanSurface vulkan;

	if (+win.Create(0, 0, 800, 600, GWindowStyle::WINDOWEDBORDERED))
	{
		VkClearValue clrAndDepth[2];
		clrAndDepth[0].color = { {0.025f, 0.095f, 0.155f, 0.25f} };
		clrAndDepth[1].depthStencil = { 1.0f, 0u }; win.SetWindowName("Particle System");
		msgs.Create([&](const GW::GEvent& e) {
			GW::SYSTEM::GWindow::Events q;
			if (+e.Read(q) && q == GWindow::Events::RESIZE)
				clrAndDepth[0].color.float32[2] += 0.00f; // disable
			});
		win.Register(msgs);
#ifndef NDEBUG
		const char* debugLayers[] = {
			"VK_LAYER_KHRONOS_validation", // standard validation layer
		};
		const char* setting_debug_action[] = { // debug actions for the validation layer
			"VK_DBG_LAYER_ACTION_LOG_MSG",
			"VK_DBG_LAYER_ACTION_DEBUG_OUTPUT",
			"VK_DBG_LAYER_ACTION_BREAK"
		};
		const VkLayerSettingEXT lyr_sett = { // extended settings for the validation layer
			debugLayers[0], "debug_action", VK_LAYER_SETTING_TYPE_STRING_EXT,
			static_cast<uint32_t>(std::size(setting_debug_action)), setting_debug_action
		};
		if (+vulkan.Create(win, 0, sizeof(debugLayers) / sizeof(debugLayers[0]),
			debugLayers, 1, &lyr_sett, 0, nullptr, 0, nullptr, false))
#else
		if (+vulkan.Create(win, GW::GRAPHICS::DEPTH_BUFFER_SUPPORT))
#endif
		{

			//Renderer renderer(win, vulkan);
			while (+win.ProcessWindowEvents())
			{
				if (+vulkan.StartFrame(2, clrAndDepth))
				{
					//renderer.Render();
					vulkan.EndFrame(true);
				}
			}
		}
	}
	return 0; // that's all folks
}

int main()
{
	std::cout << "Hello, World!\n" << std::endl;

	App app;

	return -1; // failed to create Gateware application
}
#pragma once
#include "../precompiled.h"
#include "../Renderer_solo.h"


namespace APP {

	class DefaultApp
	{
		GW::SYSTEM::UNIVERSAL_WINDOW_HANDLE* m_windowHandle;
		GWindow win;
		GEventResponder msgs;
		GVulkanSurface vulkan;
		VkClearValue clrAndDepth[2];

	public:

		DefaultApp();
		~DefaultApp();
		int Run();
	};

}
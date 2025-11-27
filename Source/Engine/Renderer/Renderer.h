#pragma once
#include "../../precompiled.h"
#include "DrawComponents.h"
#include "../../Utils/UniversalComponents.h"

namespace Engine {
	using namespace DRAW;

	class Renderer {
	public:
		Renderer(const RendererConfig& init, entt::registry& registry);
		void RenderFrame(float deltaTime);
		VulkanRenderer& GetVulkanRenderer();
		RendererConfig& GetRendererConfig();

		void InitializeRenderer();

		void UpdateRenderer(entt::registry& registry);
		void ShutdownRenderer(entt::registry& registry, UNIVERSAL::LogComponent& logger);
		void BeginFrame();
		void EndFrame();

		VkInstance GetInstance();
		uint32_t GetGraphicsQueueFamilyIndex();
		VkQueue GetGraphicsQueue();
		VkDescriptorPool& GetImGuiDescriptorPool();
		uint32_t& GetMinImageCount();
		uint32_t& GetSwapchainImageCount();
		VkSampleCountFlags GetMSAASamples();
		static void CheckVk(VkResult result);

	private:

		VkViewport CreateViewportFromWindowDimensions(unsigned int windowWidth, unsigned int windowHeight);
		VkRect2D CreateScissorFromWindowDimensions(unsigned int windowWidth, unsigned int windowHeight);
		void InitializeDescriptors();
		void InitializeGraphicsPipeline();

		VulkanRenderer m_renderer;
		RendererConfig m_initData;
		entt::registry& registry;
	};


} // namespace DRAW

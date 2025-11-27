#pragma once
#include "../../precompiled.h"
#include "DrawComponents.h"

namespace Engine {
	using namespace DRAW;

	class Renderer {
	public:
		Renderer(const RendererConfig& init);
		void RenderFrame(float deltaTime);
		VulkanRenderer& GetVulkanRenderer();
		RendererConfig& GetRendererConfig();

		void InitializeRenderer();

		void UpdateRenderer(entt::registry& registry, entt::entity entity);
		void ShutdownRenderer(entt::registry& registry, entt::entity entity);
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
	};


} // namespace DRAW

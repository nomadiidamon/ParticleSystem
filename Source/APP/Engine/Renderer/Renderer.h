#pragma once
#include "../../../precompiled.h"
#include "DRAW/DrawComponents.h"

namespace Engine {
	using namespace DRAW;

	class Renderer {
	public:
		Renderer(entt::registry& registry, VulkanRendererInitialization& init);
		void RenderFrame(float deltaTime);
		VulkanRenderer& GetVulkanRenderer();
		void InitializeRenderer();
		void BeginFrame();
		void RenderEntities();
		void EndFrame();
		VkInstance& GetInstance();
		VkDevice& GetDevice();
		VkPhysicalDevice& GetPhysicalDevice();
		uint32_t& GetGraphicsQueueFamilyIndex();
		VkQueue& GetGraphicsQueue();
		VkPipelineCache& GetPipelineCache();
		VkDescriptorPool& GetImGuiDescriptorPool();
		uint32_t& GetMinImageCount();
		uint32_t& GetSwapchainImageCount();
		VkSampleCountFlags& GetMSAASamples();
		static void CheckVk(VkResult result);


	private:
		VulkanRenderer m_renderer;
		entt::registry& m_registry;
		VulkanRendererInitialization& m_initData;
	};
	void OnConstruct_Renderer(entt::registry& registry, entt::entity entity);
	void OnUpdate_Renderer(entt::registry& registry, entt::entity entity);
	void OnDestroy_Renderer(entt::registry& registry, entt::entity entity);

} // namespace DRAW

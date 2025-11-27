#pragma once
#include "../../precompiled.h"
#include "../../App/ParticleSystemApp.h"
#include "Renderer.h"

namespace Engine {

	Renderer::Renderer(const RendererConfig& init)	: m_initData(init) {	}
	
	VkViewport Renderer::CreateViewportFromWindowDimensions(unsigned int windowWidth, unsigned int windowHeight)
	{
		VkViewport retval{};
		retval.x = 0.0f;
		retval.y = 0.0f;
		retval.width = static_cast<float>(windowWidth);
		retval.height = static_cast<float>(windowHeight);
		retval.minDepth = 0.0f;
		retval.maxDepth = 1.0f;
		return retval;

	}
	VkRect2D Renderer::CreateScissorFromWindowDimensions(unsigned int windowWidth, unsigned int windowHeight)
	{
		VkRect2D retval = {};
		retval.offset.x = 0;
		retval.offset.y = 0;
		retval.extent.width = windowWidth;
		retval.extent.height = windowHeight;
		return retval;
	}
	void Renderer::InitializeDescriptors()
	{


	}
	void Renderer::InitializeGraphicsPipeline()
	{	 
		
	}

	void Renderer::InitializeRenderer()
	{
		std::cout << "Initializing Renderer..." << std::endl;
	}
	void Renderer::UpdateRenderer(entt::registry& registry, entt::entity entity) {
		
	}
	void Renderer::ShutdownRenderer(entt::registry& registry, entt::entity entity) {
		
		std::cout << "Shutting Down Renderer..." << std::endl;
	}

#pragma region Getters
	VulkanRenderer& Renderer::GetVulkanRenderer()
	{
		return m_renderer;
	}
	RendererConfig& Renderer::GetRendererConfig()
	{
		return m_initData;
	}
	VkInstance Renderer::GetInstance()
	{
		VkInstance instance = VK_NULL_HANDLE;
		m_renderer.vlkSurface.GetInstance((void**)&instance);
		return instance;
	}
	uint32_t Renderer::GetGraphicsQueueFamilyIndex()
	{
		uint32_t queueFamilyIndexPtr = 0;
		uint32_t _ = 0;
		m_renderer.vlkSurface.GetQueueFamilyIndices(queueFamilyIndexPtr, _);
		return queueFamilyIndexPtr;

	}
	VkQueue Renderer::GetGraphicsQueue()
	{
		VkQueue graphicsQueuePtr = VK_NULL_HANDLE;
		m_renderer.vlkSurface.GetGraphicsQueue((void**)&graphicsQueuePtr);
		return graphicsQueuePtr;
	}
	VkDescriptorPool& Renderer::GetImGuiDescriptorPool()
	{
		return m_renderer.imgui.pool;
	}
	uint32_t& Renderer::GetMinImageCount()
	{
		return m_renderer.core.swapchain.minImageCount;
	}
	uint32_t& Renderer::GetSwapchainImageCount()
	{
		return m_renderer.core.swapchain.actualImageCount;
	}
	VkSampleCountFlags Renderer::GetMSAASamples()
	{
		return VK_SAMPLE_COUNT_1_BIT;
	}
	void Renderer::CheckVk(VkResult result)
	{
		if (result != VK_SUCCESS)
		{
			std::cout << "Vulkan Error: " << result << std::endl;
			abort();
		}
	}
#pragma endregion
}

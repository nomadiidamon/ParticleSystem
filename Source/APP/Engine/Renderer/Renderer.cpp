#pragma once
#include "../../../precompiled.h"
#include "../../ParticleSystemApp.h"
#include "Renderer.h"

namespace Engine {

	Renderer::Renderer(entt::registry& registry, VulkanRendererInitialization& init)
		: m_registry(registry), m_initData(init) {
	}

	void Renderer::RenderFrame(float deltaTime) {
		BeginFrame();
		RenderEntities();
		EndFrame();
	}

	VulkanRenderer& Renderer::GetVulkanRenderer()
	{
		return m_renderer;
	}

	void Renderer::InitializeRenderer()
	{
		auto& app = m_registry.get<APP::ParticleSystemApp>(m_registry.view<APP::ParticleSystemApp>().front());
		m_renderer.startTime = app.GetTime().TotalTime();
	}

	void Renderer::BeginFrame() {

	}

	void Renderer::RenderEntities() {

	}

	void Renderer::EndFrame() {

	}

	VkInstance& Renderer::GetInstance()
	{
		// TODO: insert return statement here
	}

	VkDevice& Renderer::GetDevice()
	{
		// TODO: insert return statement here
	}

	VkPhysicalDevice& Renderer::GetPhysicalDevice()
	{
		// TODO: insert return statement here
	}

	uint32_t& Renderer::GetGraphicsQueueFamilyIndex()
	{
		// TODO: insert return statement here
	}

	VkQueue& Renderer::GetGraphicsQueue()
	{
		// TODO: insert return statement here
	}

	VkPipelineCache& Renderer::GetPipelineCache()
	{
		// TODO: insert return statement here
	}

	VkDescriptorPool& Renderer::GetImGuiDescriptorPool()
	{
		// TODO: insert return statement here
	}

	uint32_t& Renderer::GetMinImageCount()
	{
		// TODO: insert return statement here
	}

	uint32_t& Renderer::GetSwapchainImageCount()
	{
		// TODO: insert return statement here
	}

	VkSampleCountFlags& Renderer::GetMSAASamples()
	{
		// TODO: insert return statement here
	}

	void Renderer::CheckVk(VkResult result)
	{
	}

	void OnConstruct_Renderer(entt::registry& registry, entt::entity entity)
	{
		auto& renderer = registry.get<Renderer>(entity);
		renderer.InitializeRenderer();
	}

	void OnUpdate_Renderer(entt::registry& registry, entt::entity entity)
	{
		auto& renderer = registry.get<Renderer>(entity);
		renderer.RenderFrame(0.0f);
	}

	void OnDestroy_Renderer(entt::registry& registry, entt::entity entity)
	{
		// Cleanup logic if needed
	}



	CONNECT_COMPONENT_LOGIC() {
		registry.on_construct<Renderer>().connect<OnConstruct_Renderer>();
		registry.on_update<Renderer>().connect<OnUpdate_Renderer>();
		registry.on_destroy<Renderer>().connect<OnDestroy_Renderer>();
	}

}

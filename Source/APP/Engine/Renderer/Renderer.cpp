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
		std::cout << "Renderer initialized!" << std::endl;
	}

	void Renderer::BeginFrame() {

	}

	void Renderer::RenderEntities() {

	}

	void Renderer::EndFrame() {

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
		std::cout << "Renderer destroyed!" << std::endl;
	}



	CONNECT_COMPONENT_LOGIC() {
		registry.on_construct<Renderer>().connect<OnConstruct_Renderer>();
		registry.on_update<Renderer>().connect<OnUpdate_Renderer>();
		registry.on_destroy<Renderer>().connect<OnDestroy_Renderer>();
	}

}

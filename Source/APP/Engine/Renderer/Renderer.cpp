#pragma once
#include "../../../precompiled.h"
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
	}

	void Renderer::BeginFrame() {

	}

	void Renderer::RenderEntities() {

	}

	void Renderer::EndFrame() {

	}

}

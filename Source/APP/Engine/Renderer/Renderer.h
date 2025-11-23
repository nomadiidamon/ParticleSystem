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



	private:
		VulkanRenderer m_renderer;
		entt::registry& m_registry;
		VulkanRendererInitialization& m_initData;
	};
	void OnConstruct_Renderer(entt::registry& registry, entt::entity entity);
	void OnUpdate_Renderer(entt::registry& registry, entt::entity entity);
	void OnDestroy_Renderer(entt::registry& registry, entt::entity entity);

} // namespace DRAW

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
    private:
        entt::registry& m_registry;
        VulkanRendererInitialization& m_initData;
        VulkanRenderer m_renderer;
		void InitializeRenderer();
        void BeginFrame();
        void RenderEntities();
        void EndFrame();
    };

} // namespace DRAW

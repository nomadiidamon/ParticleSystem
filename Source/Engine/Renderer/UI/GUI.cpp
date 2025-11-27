#pragma once 
#include "../../../precompiled.h"
#include "../../../App/Window.hpp"
#include "../DrawComponents.h"
#include "../Renderer.h"
#include "GUI.h"

namespace Engine {
	namespace UI {

		GUIManager::GUIManager(entt::registry& registry) : m_registry(registry) {

		}

		GUIManager::~GUIManager() {

		}

		void GUIManager::Update(float deltaTime) {
			/// TODO: add callbacks to system events if needed
		}

		void GUIManager::RenderFrame(float deltaTime)
		{
			BeginFrame();
			RenderGUI();   // Loop all UILayers
			EndFrame();
		}

		void GUIManager::Init() {
			auto appEntitty = m_registry.view<APP::Window>().front();
			auto& windowComp = m_registry.get<APP::Window>(appEntitty);
			auto& gwWindow = m_registry.get<GW::SYSTEM::GWindow>(appEntitty);
			GW::SYSTEM::UNIVERSAL_WINDOW_HANDLE gwHandle;
			Renderer& renderer = m_registry.get<Renderer>(m_registry.view<Renderer>().front());
			gwWindow.GetWindowHandle(gwHandle);
			printf("GUIManager initialized (Vulkan + Win32)\n");
		}

		void GUIManager::Shutdown() {
			/// TODO: Shutdown renderer backends
		}

		void GUIManager::BeginFrame() {
			/// Start new frame for renderer backends
		}

		void GUIManager::RenderGUI()
		{
			/// TODO: loop through ui layers and render them
			for (auto& layer : m_uiLayers) {
				layer->Begin();
				layer->Render(0.f);
				layer->End();
			}

			/// TODO: loop through UIRenderable components and render them
		}

		void GUIManager::EndFrame()
		{
			/// TODO: Submit draw data to renderer
		}

		void GUIManager::RenderLayers(float deltaTime)
		{
		}

		void GUIManager::UpdateLayers(float deltaTime)
		{
		}

		void OnConstruct_GUIManager(entt::registry& registry, entt::entity entity)
		{
			auto& guiManagerComp = registry.get<GUIManagerComponent>(entity);
			guiManagerComp.manager.Init();
		}

		void OnUpdate_GUIManager(entt::registry& registry, entt::entity entity)
		{
			auto& guiManagerComp = registry.get<GUIManagerComponent>(entity);
			guiManagerComp.manager.Update(0.f);
		}

		void OnDestroy_GUIManager(entt::registry& registry, entt::entity entity)
		{
			auto& guiManagerComp = registry.get<GUIManagerComponent>(entity);
			guiManagerComp.manager.Shutdown();
		}


		CONNECT_COMPONENT_LOGIC() {
			registry.on_construct<GUIManager>().connect<OnConstruct_GUIManager>();
			registry.on_update<GUIManager>().connect<OnUpdate_GUIManager>();
			registry.on_destroy<GUIManager>().connect<OnDestroy_GUIManager>();
		}


	} // namespace GUI
}
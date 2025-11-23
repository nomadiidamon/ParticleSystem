#pragma once 
#include "../../../precompiled.h"
#include "GUI.h"

namespace Engine {
	namespace UI {

		GUIManager::GUIManager(entt::registry& registry) : m_registry(registry) {

		}
		GUIManager::~GUIManager() {

		}
		void GUIManager::Update(float deltaTime) {

		}
		void GUIManager::RenderFrame(float deltaTime)
		{
		}
		void GUIManager::Init() {

		}
		void GUIManager::Shutdown() {

		}

		void GUIManager::BeginFrame()
		{
		}

		void GUIManager::RenderGUI()
		{
		}

		void GUIManager::EndFrame()
		{
		}

		void OnConstruct_GUIManager(entt::registry& registry, entt::entity entity)
		{
		}

		void OnUpdate_GUIManager(entt::registry& registry, entt::entity entity)
		{
		}

		void OnDestroy_GUIManager(entt::registry& registry, entt::entity entity)
		{
		}


		CONNECT_COMPONENT_LOGIC() {
			registry.on_construct<GUIManager>().connect<OnConstruct_GUIManager>();
			registry.on_update<GUIManager>().connect<OnUpdate_GUIManager>();
			registry.on_destroy<GUIManager>().connect<OnDestroy_GUIManager>();
		}


	} // namespace GUI
}
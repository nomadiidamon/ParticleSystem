#pragma once
#include "../../../precompiled.h"
#include "UILayer.h"

namespace Engine {
	namespace UI {
		UILayer::UILayer(entt::registry& registry) : m_registry(registry) {

		}
		UILayer::~UILayer() {

		}
		void UILayer::OnAttach() {


		}
		void UILayer::OnDetach() {

		}
		void UILayer::Begin() {
			// default to new ImGui Window
			ImGui::Begin("UI Layer");

			ImGui::End();
		}
		void UILayer::Render(float deltaTime) {

		}
		void UILayer::End() {

		}
		void UILayer::Update(float deltaTime) {

		}
		void OnConstruct_UILayer(entt::registry& registry, entt::entity entity)
		{
		}
		void OnUpdate_UILayer(entt::registry& registry, entt::entity entity)
		{
		}
		void OnDestroy_UILayer(entt::registry& registry, entt::entity entity)
		{
		}

		CONNECT_COMPONENT_LOGIC() {
			registry.on_construct<UILayerComponent>().connect<OnConstruct_UILayer>();
			registry.on_update<UILayerComponent>().connect<OnUpdate_UILayer>();
			registry.on_destroy<UILayerComponent>().connect<OnDestroy_UILayer>();
		}
	}
}
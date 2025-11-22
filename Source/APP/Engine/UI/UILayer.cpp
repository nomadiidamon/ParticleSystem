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
	}
}
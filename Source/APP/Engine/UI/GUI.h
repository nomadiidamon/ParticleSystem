#pragma once
#include "../../../precompiled.h"
#include "../Utils/UniversalComponents.h"
#include "UILayer.h"

namespace Engine {

	namespace UI {
		/// TODO: Convert this class to a particle system Editor
		class GUIManager {
		public:
			GUIManager(entt::registry& registry);
			~GUIManager();
			void Update(float deltaTime);
			void RenderFrame(float deltaTime);
			void Init();
			void Shutdown();


		private:
			entt::registry& m_registry;
			std::vector<UILayer*> m_uiLayers;
			bool m_isInitialized = false;

			void BeginFrame();
			void RenderGUI();
			void EndFrame();
			void RenderLayers(float deltaTime);
			void UpdateLayers(float deltaTime);
		};

		struct GUIManagerComponent {
			GUIManager manager;
		};

		void OnConstruct_GUIManager(entt::registry& registry, entt::entity entity);
		void OnUpdate_GUIManager(entt::registry& registry, entt::entity entity);
		void OnDestroy_GUIManager(entt::registry& registry, entt::entity entity);
	} // namespace GUI

}
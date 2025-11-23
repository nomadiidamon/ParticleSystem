#pragma once
#include "../../../precompiled.h"
#include "../Utils/UniversalComponents.h"

namespace Engine {

	namespace UI {
		class GUIManager {
		public:
			GUIManager(entt::registry& registry);
			~GUIManager();
			void Update(float deltaTime);
			void RenderFrame(float deltaTime);
			void Init();


		private:
			entt::registry& m_registry;

			void Shutdown();
			void BeginFrame();
			void RenderGUI();
			void EndFrame();
		};

		void OnConstruct_GUIManager(entt::registry& registry, entt::entity entity);
		void OnUpdate_GUIManager(entt::registry& registry, entt::entity entity);
		void OnDestroy_GUIManager(entt::registry& registry, entt::entity entity);
	} // namespace GUI

}
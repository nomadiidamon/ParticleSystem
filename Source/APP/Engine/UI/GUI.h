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

			/// TODO: Connect component logic


		private:
			entt::registry& m_registry;
			void Init();
			void Shutdown();

			void BeginFrame();
			void RenderGUI();
			void EndFrame();
		};
	} // namespace GUI

}
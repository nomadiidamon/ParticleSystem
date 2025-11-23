#pragma once 
#include "../../../precompiled.h"

namespace Engine {
	namespace UI {
		class UILayer {
		public:
			UILayer(entt::registry& registry);
			~UILayer();
			
			virtual void OnAttach();
			virtual void OnDetach();

			virtual void Begin();
			virtual void Render(float deltaTime);
			virtual void End();

			virtual	void Update(float deltaTime);
		private:
			entt::registry& m_registry;

		};

		struct UILayerComponent {
			UILayer layer;
		};

		void OnConstruct_UILayer(entt::registry& registry, entt::entity entity);
		void OnUpdate_UILayer(entt::registry& registry, entt::entity entity);
		void OnDestroy_UILayer(entt::registry& registry, entt::entity entity);

	}
}
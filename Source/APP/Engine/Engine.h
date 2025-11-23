#pragma once
#include "../../precompiled.h"
#include "Renderer/DRAW/DrawComponents.h"
#include "Renderer/Renderer.h"

namespace Engine {

    class Engine {
    public:
        Engine(entt::registry& registry);
        ~Engine();
        void Init();
        void Update(float deltaTime);
        void Shutdown();
        entt::registry& m_registry;


    private:
		bool m_isInitialized = false;
    };

	void OnConstruct_Engine(entt::registry& registry, entt::entity entity);
    void OnUpdate_Engine(entt::registry& registry, entt::entity entity);
    void OnDestroy_Engine(entt::registry& registry, entt::entity entity);

} // namespace UNIVERSAL
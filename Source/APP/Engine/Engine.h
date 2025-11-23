#pragma once
#include "../../precompiled.h"
#include "Renderer/DRAW/DrawComponents.h"
#include "Renderer/Renderer.h"

namespace Engine {

    class Engine {
    public:
        Engine(entt::registry& registry);
        ~Engine();
        void Update(float deltaTime);
        void Init();
        void Shutdown();
        void CleanupEntities();
        entt::registry& m_registry;

    private:
		bool m_isInitialized = false;
    };

} // namespace UNIVERSAL
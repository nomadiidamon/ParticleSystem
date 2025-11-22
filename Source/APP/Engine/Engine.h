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

    private:
        entt::registry& m_registry;
        void Init();
        void CleanupEntities();
        void Shutdown();
    };

} // namespace UNIVERSAL
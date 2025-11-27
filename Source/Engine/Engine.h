#pragma once
#include "../precompiled.h"
#include "Renderer/DrawComponents.h"
#include "Renderer/Renderer.h"

namespace Engine {

    class Engine {
    public:

        Engine();
        ~Engine();
        void Init();
        int UpdateEngine();
        void Shutdown();

		Renderer m_renderer;
    private:
		bool m_isInitialized = false;
        bool m_IsRunning = false;
        float m_FixedInterval = 0.02f; // 50 FPS
        float m_backgroundInterval = 0.1f; // 10 FPS
		float m_AccumulatedTime = 0.0f;
        XTime m_Time;
        int Update(entt::registry& registry);
        int FixedUpdate(entt::registry& registry);
    };

} // namespace UNIVERSAL
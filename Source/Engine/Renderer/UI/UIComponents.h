#pragma once
#include "../../../precompiled.h"

namespace IMGUI_UI
{
    //******************************************************//
    // TAGS
    //******************************************************//

    // Marks an entity that should show a general ImGui panel
    struct ImguiEnabledTag {};

    // Marks an entity that should show debug info
    struct ImguiDebugTag {};

    // Hides ImGui for this entity regardless of other tags
    struct ImguiHiddenTag {};

    //******************************************************//
    // GENERIC UI PANELS
    //******************************************************//

    struct ImguiNamePanel
    {
        bool show = true;
    };

    struct ImguiTransformPanel
    {
        bool show = true;
        bool allowEditing = true;
    };

    struct ImguiVelocityPanel
    {
        bool show = true;
        bool allowEditing = true;
    };

    struct ImguiLifespanPanel
    {
        bool show = true;
    };

    struct ImguiColorPanel
    {
        bool show = true;
        bool enableGradientPreview = true;
        bool allowEditing = true;
    };

    //******************************************************//
    // PARTICLE UI
    //******************************************************//

    struct ImguiParticleSystemPanel
    {
        bool show = true;

        // basic particle system control
        bool emit = true;
        float emitRate = 20.0f;
        float particleSize = 0.1f;

        // debug visualization
        bool showBounds = false;
        bool showParticles = true;
    };

    struct ImguiParticleComponentPanel
    {
        bool show = true;
        bool allowEditing = true;

        // runtime info
        bool showRuntimeData = true;
    };

    //******************************************************//
    // DRAW / VULKAN / RENDERER UI
    //******************************************************//

    struct ImguiRendererPanel
    {
        bool show = true;
        bool showSwapchainInfo = true;
        bool showMemoryStats = true;
        bool showShaders = true;
        bool allowReloadShaders = false;
        bool showRenderPassInfo = false;
    };

    struct ImguiCameraPanel
    {
        bool show = true;
        bool allowEditing = true;
        bool showFrustum = false;
    };

    struct ImguiLightPanel
    {
        bool show = true;
        bool allowEditing = true;
        bool enableColorPicker = true;
    };

    //******************************************************//
    // DEBUG PANELS
    //******************************************************//

    struct ImguiEntityInspector
    {
        bool show = true;

        // focus on a specific entity
        entt::entity inspected = entt::null;
    };

    struct ImguiStatsPanel
    {
        bool show = true;

        float fps = 0.f;
        float frameTime = 0.f;
        uint32_t drawCalls = 0;
        uint32_t particleCount = 0;
        uint32_t entityCount = 0;
    };

} // namespace IMGUI_UI

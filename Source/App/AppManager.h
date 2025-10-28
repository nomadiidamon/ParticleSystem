#pragma once
#include "../precompiled.h"

enum AppWindowMode : uint8_t
{
	APP_MODE_WINDOWED,
	APP_MODE_FULLSCREEN,
	APP_MODE_BORDERLESS
};

enum AppQuality : uint8_t
{
	APP_QUALITY_LOW,
	APP_QUALITY_MEDIUM,
	APP_QUALITY_HIGH,
	APP_QUALITY_ULTRA
};

enum AppPerformance : uint8_t
{
	APP_PERFORMANCE_BATTERY,
	APP_PERFORMANCE_BALANCED,
	APP_PERFORMANCE_HIGH
};

class AppManager {
public:

	AppManager();
	~AppManager();
	AppWindowMode windowMode = APP_MODE_WINDOWED;
	AppQuality quality = APP_QUALITY_HIGH;
	AppPerformance performance = APP_PERFORMANCE_BALANCED;

	entt::registry m_registry;

	// Local routines for specific application behavior
	void GraphicsBehavior(entt::registry& registry);
	void GameplayBehavior(entt::registry& registry);
	void MainLoopBehavior(entt::registry& registry);

};
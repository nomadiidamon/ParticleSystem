#pragma once
#include "../precompiled.h"
#include "AppManager.h"

enum AppState : uint8_t
{
	APP_LOADING,
	APP_RUNNING,
	APP_UNFOCUSED,
	APP_MINIMIZED,
	APP_EXITING
};

enum AppUpdateStatus : uint8_t
{
	APP_UPDATE_OK = 0,
	APP_UPDATE_ERROR = 5,
	APP_UPDATE_EXIT = 10
};



class App
{
	AppManager manager;
	GW::SYSTEM::UNIVERSAL_WINDOW_HANDLE*  m_windowHandle;
	GWindow win;
	GEventResponder msgs;
	GVulkanSurface vulkan;
	VkClearValue clrAndDepth[2];
	AppState m_state = APP_RUNNING;
	AppUpdateStatus m_updateStatus = APP_UPDATE_OK;
	AppUpdateStatus m_fixedUpdateStatus = APP_UPDATE_OK;
	float m_FixedInterval = 0.02f; // 50 FPS
	float m_backgroundInterval = 0.1f; // 10 FPS


public:

	App();
	~App();

	int Run();

	int Update();
	int FixedUpdate();


private:
	int HandleUpdate();
	int HandleFixedUpdate();


};
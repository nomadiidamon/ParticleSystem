#pragma once

namespace APP {

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
}
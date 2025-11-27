#pragma once
#include "../precompiled.h"

namespace UNIVERSAL {
	///*** Tags ***///
	struct DoNotRenderTag {}; // Used to mark entities that should be excluded from rendering
	struct InactiveTag {}; // Used to mark entities that are inactive and wont be updated
	struct StaticTag {}; // Used to mark entities that do not move
	struct CollisionTag {}; // Used to mark entities that participate in collision detection
	struct ObstacleTag {}; // Used to mark entities that act as obstacles in the environment and respond to collisions
	struct DestroyTag {}; // Used to mark entities that should be removed from the registry
	struct ParticleTag {}; // Used to mark entities that are particles
	struct ParticleSystemTag {}; // Used to mark entities that are particle systems
	struct CameraTag {}; // Used to mark entities that represent cameras
	struct LightTag {}; // Used to mark entities that represent lights

	///*** Components ***///
	struct IDComponent {
		uint32_t id;
	};

	/*struct GUID {
		string? value;
		uint32_Long? mostSignificantBits;
	}*/

	struct NameComponent {
		std::string name;
	};
	struct TransformComponent {
		GW::MATH::GMATRIXF transform;
	};
	struct VelocityComponent {
		GW::MATH::GVECTORF velocity;
	};
	struct LifespanComponent {
		float lifeExpectancy;
		float lifeRemaining;
	};
	struct ColorComponent {
		GW::MATH::GVECTORF startColor;
		GW::MATH::GVECTORF endColor;
		GW::MATH::GVECTORF currentColor;
	};

	enum class LogLevel {
		LOGS,
		WARNINGS,
		ERRORS
	};

	struct LogEntry {
		std::string message;
		LogLevel level;
	};

	static const std::string logPrependText = "[LOG]: ";
	static const std::string warningPrependText = "[WARNING]: ";
	static const std::string errorPrependText = "[ERROR]: ";
	struct LogComponent {

		std::vector<LogEntry> logs;
		void InitLogger(bool showExamples, bool showInfo);
		void EnableLogLevel(LogLevel level, bool enable);
		void Log(const std::string& log);
		void GreenLog(const std::string& log);
		void YellowLog(const std::string& log);
		void RedLog(const std::string& log);
		void LogWarning(const std::string& warning);
		void LogError(const std::string& error);
		void AddToQueue(const LogEntry& entry);
		void AddToQueue(const std::string& message, LogLevel level);
		void ClearLogs();
		void PrintLogs();
		void PrintLogs_IgnoreLevels();
		size_t GetCount();
		void SetConsoleColor(WORD color);
	private:
		HANDLE hConsole;
		std::map<LogLevel, bool> enabledLogLevels = {
			{ LogLevel::LOGS, true },
			{ LogLevel::WARNINGS, true },
			{ LogLevel::ERRORS, true }
		};
	};


}
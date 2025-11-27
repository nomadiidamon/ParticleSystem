#pragma once
#include "UniversalComponents.h"

namespace UNIVERSAL {
	void LogComponent::InitLogger(bool showExamples = true, bool showInfo = false)
	{
		if (showInfo) {
			std::cout << "Setting up logger..." << std::endl;
		}
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		enabledLogLevels[LogLevel::LOGS] = true;
		enabledLogLevels[LogLevel::WARNINGS] = true;
		enabledLogLevels[LogLevel::ERRORS] = true;
		if (showExamples)
		{
			Log("This is an info log example.");
			GreenLog("This is a green log example.");
			YellowLog("This is a yellow log example.");
			RedLog("This is a red log example.");
			LogWarning("This is a warning log example.");
			LogError("This is an error log example.");
		}
		if (showInfo) {
			std::cout << "Logger Initialized!\n" << std::endl;
		}
	}
	void LogComponent::EnableLogLevel(LogLevel level, bool enable)
	{
		enabledLogLevels[level] = enable;
	}
	void LogComponent::AddToQueue(const LogEntry& entry) {

		logs.push_back(entry);
	}
	void LogComponent::AddToQueue(const std::string& message, LogLevel level) {
		LogEntry entry{ message, level };
		logs.push_back(entry);
	}
	void LogComponent::Log(const std::string& log) {
		if (!enabledLogLevels[LogLevel::LOGS]) {
			return;
		}
		std::cout << logPrependText << log << std::endl;
	}
	void LogComponent::GreenLog(const std::string& log)
	{
		if (!enabledLogLevels[LogLevel::LOGS]) {
			return;
		}
		// Green text
		SetConsoleColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		std::cout << logPrependText << log << std::endl;
		SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // reset
	}
	void LogComponent::YellowLog(const std::string& log)
	{
		if (!enabledLogLevels[LogLevel::LOGS]) {
			return;
		}
		// Yellow text
		SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		std::cout << logPrependText << log << std::endl;
		SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // reset
	}
	void LogComponent::RedLog(const std::string& log)
	{
		if (!enabledLogLevels[LogLevel::LOGS]) {
			return;
		}
		// Red text
		SetConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
		std::cout << logPrependText << log << std::endl;
		SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // reset
	}
	void LogComponent::LogWarning(const std::string& warning)
	{
		if (!enabledLogLevels[LogLevel::WARNINGS]) {
			return;
		}
		// Yellow text
		SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		std::cout << warningPrependText << warning << std::endl;
		SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // reset
	}
	void LogComponent::LogError(const std::string& error)
	{
		if (!enabledLogLevels[LogLevel::ERRORS]) {
			return;
		}
		// Red text
		SetConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
		std::cerr << errorPrependText << error << std::endl;
		SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // reset
	}
	void LogComponent::ClearLogs() {
		logs.clear();
	}
	void LogComponent::PrintLogs() {

		for (const auto& entry : logs) {
			switch (entry.level) {
			case LogLevel::LOGS:
				Log(entry.message);
				break;
			case LogLevel::WARNINGS:
				LogWarning(entry.message);
				break;
			case LogLevel::ERRORS:
				LogError(entry.message);
				break;
			}
		}
	}
	void LogComponent::PrintLogs_IgnoreLevels() {
		for (auto& entry : logs) {
			switch (entry.level) {
			case LogLevel::LOGS:     std::cout << logPrependText << entry.message << "\n"; break;
			case LogLevel::WARNINGS: std::cout << warningPrependText << entry.message << "\n"; break;
			case LogLevel::ERRORS:   std::cerr << errorPrependText << entry.message << "\n"; break;
			}
		}
	}
	size_t LogComponent::GetCount() {
		return logs.size();
	}
	void LogComponent::SetConsoleColor(WORD color)
	{
		SetConsoleTextAttribute(hConsole, color);
	}
}
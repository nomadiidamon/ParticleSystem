#pragma once
#include "UniversalComponents.h"

namespace UNIVERSAL {

	void OnConstruct_LogComponent(entt::registry& registry, entt::entity entity) {
		auto& logComp = registry.get<LogComponent>(entity);
		logComp.logs.push_back("LogComponent created.");
	}

	void OnUpdate_LogComponent(entt::registry& registry, entt::entity entity) {
		auto& logComp = registry.get<LogComponent>(entity);
		logComp.logs.push_back("LogComponent updated.");
		for (const auto& log : logComp.logs) {
			std::cout << log << std::endl;
		}
		logComp.logs.clear();
	}

	void OnDestroy_LogComponent(entt::registry& registry, entt::entity entity) {
		auto& logComp = registry.get<LogComponent>(entity);
		logComp.logs.push_back("LogComponent destroyed.");
	}

	CONNECT_COMPONENT_LOGIC() {
		registry.on_construct<LogComponent>().connect<OnConstruct_LogComponent>();
		registry.on_update<LogComponent>().connect<OnUpdate_LogComponent>();
		registry.on_destroy<LogComponent>().connect<OnDestroy_LogComponent>();
	}
}
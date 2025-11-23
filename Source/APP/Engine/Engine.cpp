#pragma once
#include "Engine.h"


namespace Engine {


	Engine::Engine(entt::registry& registry) : m_registry(registry)
	{
	
	}

	Engine::~Engine()
	{
	}

	void Engine::Update(float deltaTime) {

	}

	void Engine::Init()
	{
		if (m_isInitialized) {
			return;
		}
		// Initialization logic here

		std::cout << "Engine Initialization Logic Executed." << std::endl;
		m_isInitialized = true;
	}

	void Engine::Shutdown()
	{
	}

	void OnConstruct_Engine(entt::registry& registry, entt::entity entity) {
		auto& engine = registry.get<Engine>(entity);
		engine.Init();
		std::cout << "Engine Initialized!" << std::endl;
	}

	void OnUpdate_Engine(entt::registry& registry, entt::entity entity) {
		auto& engine = registry.get<Engine>(entity);
		// engine.Update(...); // pass delta time if needed
		static int updateCount = 0;
		if (updateCount == 60) { // Log every 60 updates
			std::cout << "Engine Update Called! Count: " << updateCount << std::endl;
			updateCount = 0;
		}
		else {
			updateCount++;
		}
	}

	void OnDestroy_Engine(entt::registry& registry, entt::entity entity) {
		auto& engine = registry.get<Engine>(entity);
		std::cout << "Engine Shutting Down!" << std::endl;
		engine.Shutdown();
	}

	CONNECT_COMPONENT_LOGIC() {
		registry.on_construct<Engine>().connect<OnConstruct_Engine>();
		registry.on_update<Engine>().connect<OnUpdate_Engine>();
		registry.on_destroy<Engine>().connect<OnDestroy_Engine>();
	}

}// namespace UNIVERSAL
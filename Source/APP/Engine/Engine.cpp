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
	}


	void Engine::CleanupEntities() {

	}
	void Engine::Shutdown()
	{
		CleanupEntities();

	}
}// namespace UNIVERSAL
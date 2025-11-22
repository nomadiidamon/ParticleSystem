#include <iostream>
#include "APP/Engine/Utils/Iuvo/IuvoUtils.h"
#include "APP/DefaultApp.h"
#include "APP/ParticleSystemApp.h"
#include "precompiled.h"


int main()
{
	Iuvo::MemoryNet(); // check for memory leaks

	entt::registry registry;
    CCL::InitializeComponentLogic(registry);

    // 3D Content Creation reference
	//APP::DefaultApp defaultApp;
    
	// Particle System Application
    APP::ParticleSystemApp psApp("Particle System", 50, 50, 1000, 600, GWindowStyle::WINDOWEDBORDERED, registry);
    psApp.Run();


    // clear all entities and components from the registry
    registry.clear();
    return 0;
}
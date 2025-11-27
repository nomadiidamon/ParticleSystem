#include <iostream>
#include "Utils/IuvoUtils.h"
#include "APP/ParticleSystemApp.h"
#include "precompiled.h"

int main()
{
	Iuvo::MemoryNet(); // check for memory leaks

    CCL::InitializeComponentLogic(registry);
    
	// Particle System Application
    APP::ParticleSystemApp psApp("Particle System", 50, 50, 1000, 600, GWindowStyle::WINDOWEDBORDERED);

    psApp.Run(); // contains the main engine loop


    // clear all entities and components from the registry
    registry.clear();
    return 0;
}
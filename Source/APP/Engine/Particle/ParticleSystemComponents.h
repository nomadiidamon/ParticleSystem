#pragma once
#include "../../../precompiled.h"
#include "../Utils/UniversalComponents.h"

namespace PARTICLE {
	using namespace UNIVERSAL;

	///*** Components ***///
	struct EmissionComponent {
		int maxParticles;
		int particlesPerEmit;
		float emissionRate;
	};
	struct BurstComponent {
		int maxParticles;
		int burstCount;
		float burstInterval;
	};

	struct ParticleSystemComponent {
		TransformComponent system_transform;
		VelocityComponent system_velocity;
		LifespanComponent system_lifespan;
		ColorComponent system_color;
		EmissionComponent system_emission;
		BurstComponent system_burst;
		std::vector<entt::entity> system_particles;

		// will add properties for model, texture, size, etc. later
	};


}
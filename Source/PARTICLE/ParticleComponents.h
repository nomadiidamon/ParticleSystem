#pragma once
#include "../precompiled.h"
#include "../UTIL/UniversalComponents.h"

namespace PARTICLE {
	using namespace UNIVERSAL;

	///*** Components ***///
	struct ParticleComponent {
		TransformComponent transform;
		VelocityComponent velocity;
		LifespanComponent lifespan;
		ColorComponent color;
		DRAW::MeshCollection mesh;
		// will add properties for model, texture, size, etc. later
	};

}
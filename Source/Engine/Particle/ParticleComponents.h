#pragma once
#include "../../precompiled.h"
#include "../../Utils/UniversalComponents.h"
#include "../Renderer/DrawComponents.h"

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
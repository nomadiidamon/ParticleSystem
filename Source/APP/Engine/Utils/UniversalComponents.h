#pragma once
#include "../../../precompiled.h"

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
}
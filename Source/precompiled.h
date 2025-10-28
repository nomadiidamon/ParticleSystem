// precompiling large headers reduces compile times
// it also makes intellisense less angry and confused


//// EnTT is an entity component system (ECS) library.
// DOCUMENTATION: https://skypjack.github.io/entt/
#include "../Dependencies/entt-3.13.1/single_include/entt/entt.hpp"


//// Gateware is a modular PAL library.
// DOCUMENTATION: gateware-main/documentation/html/index.html
// we only need a few Gateware modules, so we enable just those.
#define GATEWARE_ENABLE_CORE
#define GATEWARE_ENABLE_MATH
#define GATEWARE_ENABLE_MATH2D
#define GATEWARE_ENABLE_INPUT
#define GATEWARE_ENABLE_SYSTEM
#define GATEWARE_ENABLE_GRAPHICS
// disable some graphics libs we don't need
#define GATEWARE_DISABLE_GOPENGLSURFACE
#define GATEWARE_DISABLE_GRASTERSURFACE
#define GATEWARE_DISABLE_GDIRECTX11SURFACE
#define GATEWARE_DISABLE_GDIRECTX12SURFACE
#include "../Dependencies/gateware-main-25.286.16/Gateware.h"
using namespace GW;
using namespace CORE;
using namespace SYSTEM;
using namespace GRAPHICS;

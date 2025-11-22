// precompiling large headers reduces compile times
// it also makes intellisense less angry and confused


//// EnTT is an entity component system (ECS) library.
// DOCUMENTATION: https://skypjack.github.io/entt/
#include "../Dependencies/entt-3.13.1/single_include/entt/entt.hpp"

//// Json is a popular data interchange format.
// DOCUMENTATION:
#include "../Dependencies/json/json.hpp"

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


//// utilities
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../Dependencies/Imgui/imgui.h"
#include "../Dependencies/Imgui/imgui_impl_win32.h"
#define VK_USE_PLATFORM_WIN32_KHR
#include "../Dependencies/Imgui/imgui_impl_vulkan.h"
#include <windows.h>
#define APP_USE_UNLIMITED_FRAME_RATE
#ifdef _DEBUG
#define APP_USE_VULKAN_DEBUG_REPORT
#endif


#include "shaderc/shaderc.h" // needed for compiling shaders at runtime
#ifdef _WIN32 // must use MT platform DLL libraries on windows
#pragma comment(lib, "shaderc_combined.lib") 
#endif
#include "APP/Engine/Utils/CCL.h"
#include "APP/Engine/Utils/h2bParser.h"
#include "APP/Engine/Utils/load_data_oriented.h"
#include "APP/Engine/Utils/XTime.h"

cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME ParticleSystem)
project(${PROJECT_NAME})

set(PRE_COMPILED
	./Source/precompiled.h 
)

# Create list of source files.
# CMake generally discourages the use of GLOB. (it can make doing more complex operations fail) 
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
	./Source/*.h
	./Source/*.hpp
	./Source/*.cpp
	./Shaders/*.hlsl
	./Dependencies/gateware-main-25.286.16/Gateware.h
	./Dependencies/entt-3.13.1/single_include/entt/entt.hpp
)

# force unicode in some libraries on win32
ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)

if (WIN32)
	# by default CMake selects "ALL_BUILD" as the startup project 
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
	add_executable (${PROJECT_NAME} ${SOURCE_FILES})
	source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${SOURCE_FILES})
	
	# --- Vulkan
	target_include_directories(${PROJECT_NAME} PUBLIC $ENV{VULKAN_SDK}/Include/)
	target_link_directories(${PROJECT_NAME} PUBLIC $ENV{VULKAN_SDK}/Lib/)		
			
	# shaderc_combined.lib in Vulkan requires this for debug & release (runtime shader compiling)
	target_compile_options(${PROJECT_NAME} PRIVATE "/MD")
endif(WIN32)

# EnTT requires c++17 support
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

# adding precompiled header to reduce compile times
target_precompile_headers(${PROJECT_NAME} PRIVATE ${PRE_COMPILED})

# Disable all shaders from compiling since they may have Vulkan specifics
file(GLOB_RECURSE SHADER_FILES CONFIGURE_DEPENDS
	./Shaders/*.hlsl
)
set_source_files_properties( ${SHADER_FILES} PROPERTIES
	VS_SHADER_TYPE Vertex # not all are vertex shaders, but that's ok here
    VS_SHADER_MODEL 5.1
    VS_SHADER_ENTRYPOINT main
    VS_TOOL_OVERRIDE "None" # stop VS from compiling, we will do it
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND git add --all || true
	COMMAND git commit -a -m "auto commit" || true
	COMMAND git push
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	VERBATIM USES_TERMINAL
)

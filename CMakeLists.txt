cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME ParticleSystem)
project(${PROJECT_NAME})

set(PRE_COMPILED
	./Source/precompiled.h 
)

# Create list of source files.
# CMake generally discourages the use of GLOB. (it can make doing more complex operations fail)
# For students the conveinece of not editing the CMake constantly is probably more important.  
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
	./Source/*.h
	./Source/*.hpp
	./Source/*.cpp
	./Shaders/*.hlsl
	./Dependencies/gateware-main-25.286.16/Gateware.h
	./Dependencies/entt-3.13.1/single_include/entt/entt.hpp
	./Dependencies/wxWidgets-3.3.1/include/*.h
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
	
	# --- wxWidgets 
	if (DEFINED ENV{WXWIN})
        message(STATUS "Using wxWidgets from: $ENV{WXWIN}")
		
		
	# Define the base and specific wx include dirs
    set(WX_BASE_DIR $ENV{WXWIN})
    set(WX_LIB_DIR "${WX_BASE_DIR}/lib/vc_x64_lib")
    set(WX_SETUP_DIR "${WX_LIB_DIR}/mswud")
		
    # Include directories
    target_include_directories(${PROJECT_NAME} PUBLIC
        ${WX_BASE_DIR}/include
        ${WX_SETUP_DIR}        # this is where wx/setup.h lives
    )
		
    # Link directories
    target_link_directories(${PROJECT_NAME} PUBLIC
        ${WX_LIB_DIR}
    )
		
    # Link against the debug static wx libs
    target_link_libraries(${PROJECT_NAME} PRIVATE
        wxmsw33ud_core.lib
        wxbase33ud.lib
    )

else()
    message(FATAL_ERROR "Environment variable WXWIN not set. Please set WXWIN to your wxWidgets root.")
endif()
		
	# shaderc_combined.lib in Vulkan requires this for debug & release (runtime shader compiling)
	target_compile_options(${PROJECT_NAME} PRIVATE "/MD")
endif(WIN32)

if(UNIX AND NOT APPLE)
	# libshaderc_combined.a is required for runtime shader compiling
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -lX11 -lshaderc_combined")
    find_package(X11)
	find_package(Vulkan REQUIRED)
    link_libraries(${X11_LIBRARIES})
    include_directories(${X11_INCLUDE_DIR})
    include_directories(${Vulkan_INCLUDE_DIR}) 
	#link_directories(${Vulkan_LIBRARY}) this is currently not working
	link_libraries(${Vulkan_LIBRARIES})
	# the path is (properly)hardcoded because "${Vulkan_LIBRARY}" currently does not 
	# return a proper path on MacOS (it has the .dynlib appended)
    link_libraries(/usr/lib/x86_64-linux-gnu/libshaderc_combined.a)
    add_executable (${PROJECT_NAME} ${SOURCE_FILES})
    source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${SOURCE_FILES})
endif(UNIX AND NOT APPLE)

if(APPLE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fmodules -fcxx-modules")
	set(Architecture ${CMAKE_OSX_ARCHITECTURES})
	find_package(Vulkan REQUIRED)
	include_directories(${Vulkan_INCLUDE_DIR}) 
	#link_directories(${Vulkan_LIBRARY}) this is currently not working
	link_libraries(${Vulkan_LIBRARIES})
	# libshaderc_combined.a is required for runtime shader compiling
	# the path is (properly)hardcoded because "${Vulkan_LIBRARY}" currently does not 
	# return a proper path on MacOS (it has the .dynlib appended)
	link_libraries(/usr/local/lib/libshaderc_combined.a)
	add_executable (${PROJECT_NAME} source/main.mm)
endif(APPLE)

# EnTT requires c++17 support
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

# adding gateware.h and other librairies as a precompiled headers to reduce compile times
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

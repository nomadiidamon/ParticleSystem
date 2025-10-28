cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME ParticleSystem)
project(${PROJECT_NAME})

set(PRE_COMPILED
    ./Source/precompiled.h
)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
    ./Source/*.h
    ./Source/*.hpp
    ./Source/*.cpp
    ./Shaders/*.hlsl
    ./Dependencies/gateware-main-25.286.16/Gateware.h
    ./Dependencies/entt-3.13.1/single_include/entt/entt.hpp
    ./Dependencies/json/*.hpp
    ./${CMAKE_CURRENT_SOURCE_DIR}/ParticleSystemApp/*cs
    ./${CMAKE_CURRENT_SOURCE_DIR}/ParticleSystemApp/*xaml
)

add_definitions(-DUNICODE -D_UNICODE)

if (WIN32)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
    add_executable(${PROJECT_NAME} ${SOURCE_FILES})
    source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${SOURCE_FILES})

    target_include_directories(${PROJECT_NAME} PUBLIC $ENV{VULKAN_SDK}/Include/)
    target_link_directories(${PROJECT_NAME} PUBLIC $ENV{VULKAN_SDK}/Lib/)
    target_compile_options(${PROJECT_NAME} PRIVATE "/MD")
endif()

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_precompile_headers(${PROJECT_NAME} PRIVATE ${PRE_COMPILED})

file(GLOB_RECURSE SHADER_FILES CONFIGURE_DEPENDS ./Shaders/*.hlsl)
set_source_files_properties(${SHADER_FILES} PROPERTIES
    VS_SHADER_TYPE Vertex
    VS_SHADER_MODEL 5.1
    VS_SHADER_ENTRYPOINT main
    VS_TOOL_OVERRIDE "None"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND git add --all || true
    COMMAND git commit -a -m "auto commit" || true
    COMMAND git push
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    VERBATIM USES_TERMINAL
)

# ================================================================
# 👇 Add the .NET MAUI app project into the same Visual Studio solution
# ================================================================

set(DOTNET_PROJECT_NAME "ParticleSystemApp")
set(DOTNET_PROJECT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/ParticleSystemApp/ParticleSystemApp.csproj")

if (EXISTS ${DOTNET_PROJECT_PATH})
    add_custom_target(${DOTNET_PROJECT_NAME} ALL
        COMMAND dotnet build ${DOTNET_PROJECT_PATH} /property:Configuration=$<CONFIG>
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Building .NET MAUI app: ${DOTNET_PROJECT_NAME}"
    )

    set_property(TARGET ${DOTNET_PROJECT_NAME} PROPERTY VS_DOTNET_REFERENCES "")
    set_property(TARGET ${DOTNET_PROJECT_NAME} PROPERTY PROJECT_LABEL "${DOTNET_PROJECT_NAME}")

    add_dependencies(${DOTNET_PROJECT_NAME} ${PROJECT_NAME})
else()
    message(WARNING "Could not find ${DOTNET_PROJECT_PATH}, skipping MAUI project integration.")
endif()


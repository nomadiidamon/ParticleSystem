#include "../Particle/ParticleComponents.h"
#include "../Particle/ParticleSystemComponents.h"
#include "DrawComponents.h"
#include "Renderer.h"

namespace DRAW
{
	void Construct_CPULevel(entt::registry& registry, entt::entity entity) {

	}

	void Construct_GPULevel(entt::registry& registry, entt::entity entity) {

	}

	void Destroy_MeshCollection(entt::registry& registry, entt::entity entity) {

	}

	CONNECT_COMPONENT_LOGIC() {
		// register the Window component's logic
		registry.on_construct<CPULevel>().connect<Construct_CPULevel>();
		registry.on_construct<GPULevel>().connect<Construct_GPULevel>();
		registry.on_destroy<MeshCollection>().connect<Destroy_MeshCollection>();
	}


	VkShaderModule PipelineSystem::LoadShaderToRasterPipeline(const char* path, shaderc_shader_kind kind, VulkanCoreContext& core, PipelineSystem& pipeline)
	{
		std::string shaderSource = Iuvo::ReadFileIntoString(path);
		if (shaderSource.empty()) {
			std::cerr << "Failed to read shader file: " + std::string(path) << std::endl;
			return VK_NULL_HANDLE;
		}

		shaderc_compiler_t compiler = shaderc_compiler_initialize();
		shaderc_compile_options_t options = shaderc_compile_options_initialize();
		shaderc_compile_options_set_source_language(options, shaderc_source_language_hlsl);
		shaderc_compile_options_set_invert_y(options, false);
#ifndef NDEBUG
		shaderc_compile_options_set_generate_debug_info(options);
#endif

		shaderc_compilation_result_t result = shaderc_compile_into_spv( // compile
			compiler, shaderSource.c_str(), shaderSource.length(),
			kind, "main.vert", "main", options);

		if (shaderc_result_get_compilation_status(result) != shaderc_compilation_status_success) // errors?
		{
			std::cout << "Vertex Shader Errors : \n" << shaderc_result_get_error_message(result) << std::endl;
			abort();
			return VK_NULL_HANDLE;
		}

		GvkHelper::create_shader_module(core.device.device, shaderc_result_get_length(result), // load into Vulkan
			(char*)shaderc_result_get_bytes(result), &pipeline.rasterPipeline.vertShader);

		shaderc_result_release(result); // done
		return pipeline.rasterPipeline.vertShader;
	}

	VkShaderModule LoadShaderToComputePipeline(const char* path, shaderc_shader_kind kind, VulkanCoreContext& core, PipelineSystem& pipeline)
	{
		std::string shaderSource = Iuvo::ReadFileIntoString(path);
		if (shaderSource.empty()) {
			std::cerr << "Failed to read shader file: " + std::string(path) << std::endl;
			return VK_NULL_HANDLE;
		}
		shaderc_compiler_t compiler = shaderc_compiler_initialize();
		shaderc_compile_options_t options = shaderc_compile_options_initialize();
		shaderc_compile_options_set_source_language(options, shaderc_source_language_hlsl);
		shaderc_compile_options_set_invert_y(options, false);
#ifndef NDEBUG
		shaderc_compile_options_set_generate_debug_info(options);
#endif
		shaderc_compilation_result_t result = shaderc_compile_into_spv( // compile
			compiler, shaderSource.c_str(), shaderSource.length(),
			kind, "main.comp", "main", options);
		if (shaderc_result_get_compilation_status(result) != shaderc_compilation_status_success) // errors?
		{
			std::cout << "Compute Shader Errors : \n" << shaderc_result_get_error_message(result) << std::endl;
			abort();
			return VK_NULL_HANDLE;
		}
		Engine::Renderer::CheckVk(GvkHelper::create_shader_module(core.device.device, shaderc_result_get_length(result), // load into Vulkan
			(char*)shaderc_result_get_bytes(result), &pipeline.computePipeline.computeShader));
		shaderc_result_release(result); // done
		return pipeline.computePipeline.computeShader;
	}

}
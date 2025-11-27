#include "../../precompiled.h"
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


	/// TODO: Implement texture loading
	VulkanTexture TextureManager::CreateTextureFromFile(const std::string& path)
	{
		return VulkanTexture();
	}

	/// TODO: Implement texture loading
	VulkanTexture TextureManager::CreateTextureFromMemory()
	{
		return VulkanTexture();
	}

	VulkanBuffer BufferManager::CreateVertexIndexBuffer(const VkPhysicalDevice& physicalDevice, const VkDevice& device, VkDeviceSize& vertSize, VkDeviceSize& indexSize) {
		VulkanBuffer out{};

		// Create vertex buffer
		GvkHelper::create_buffer( 
			physicalDevice, device, vertSize,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			&out.vertexBuffer.buffer, &out.vertexBuffer.memory);

		// Create index buffer
		GvkHelper::create_buffer( 
			physicalDevice, device, indexSize,
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			&out.indexBuffer.buffer, &out.indexBuffer.memory);
		geometry.push_back(out);
		return out;
	}
	
	/// TODO: Implement uniform buffer creation
	VulkanUniformBuffer BufferManager::CreateUniformBuffers(size_t size, uint32_t count)
	{
		return VulkanUniformBuffer();
	}

	/// TODO: Implement instance buffer creation
	VulkanInstanceBuffer BufferManager::CreateInstanceBuffer(uint32_t count)
	{
		return VulkanInstanceBuffer();
	}

	bool DescriptorSystem::CreateLayouts(VkDevice& device)
	{
		VkDescriptorSetLayoutBinding uboBinding{};
		uboBinding.binding = 0;
		uboBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboBinding.descriptorCount = 1;
		uboBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = 1;
		layoutInfo.pBindings = &uboBinding;

		return vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &layout) == VK_SUCCESS;
	}

	bool DescriptorSystem::AllocateSets(VkDevice& device, uint32_t count)
	{
		std::vector<VkDescriptorSetLayout> layouts(count, layout);

		VkDescriptorSetAllocateInfo alloc{};
		alloc.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		alloc.descriptorPool = pool;
		alloc.descriptorSetCount = count;
		alloc.pSetLayouts = layouts.data();

		sets.resize(count);
		return vkAllocateDescriptorSets(device, &alloc, sets.data()) == VK_SUCCESS;
	}

	bool DescriptorSystem::UpdateSceneUBODescriptors(VkDevice& device, VulkanUniformBuffer& ubo)
	{
		for (uint32_t i = 0; i < ubo.bufferCount; i++)
		{
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = ubo.buffer[i];
			bufferInfo.offset = 0;
			bufferInfo.range = ubo.alignedSize;

			VkWriteDescriptorSet write{};
			write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write.dstSet = sets[i];
			write.dstBinding = 0;
			write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			write.descriptorCount = 1;
			write.pBufferInfo = &bufferInfo;

			vkUpdateDescriptorSets(device, 1, &write, 0, nullptr);
		}
		return true;
	}

	bool RenderResources::Initialize(VulkanCoreContext& core) {
		// Create scene UBO
		sceneUniformBuffer = buffers.CreateUniformBuffers(sizeof(SceneData), core.swapchain.actualImageCount);

		// Create instance buffer
		instanceBuffer = buffers.CreateInstanceBuffer(1000);

		// Particle storage if enabled
		particleStorage.count = 0; // filled in compute module later

		return true;
	}

	void RenderResources::Cleanup(VkDevice& device) {
		for (auto& g : buffers.geometry)
		{
			vkDestroyBuffer(device, g.vertexBuffer.buffer, nullptr);
			vkFreeMemory(device, g.vertexBuffer.memory, nullptr);
			vkDestroyBuffer(device, g.indexBuffer.buffer, nullptr);
			vkFreeMemory(device, g.indexBuffer.memory, nullptr);
		}
	}

	/// TODO: Implement pipeline creation
	bool PipelineSystem::CreateRaster(const PipelineConfig& config, VulkanCoreContext& core, const DescriptorSystem& desc)
	{
		// 1. Load shaders ------------------------------------
		rasterPipeline.vertShader = LoadShaderToRasterPipeline(config.vertexShaderName.c_str(),
			shaderc_vertex_shader, core, *this);
		rasterPipeline.fragShader = LoadShaderToRasterPipeline(config.fragmentShaderName.c_str(),
			shaderc_fragment_shader,
			core, *this);

		// 2. Pipeline layout ---------------------------------
		VkPipelineLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		layoutInfo.setLayoutCount = 1;
		layoutInfo.pSetLayouts = &desc.layout;

		vkCreatePipelineLayout(core.device.device, &layoutInfo, nullptr, &rasterPipeline.layout);

		// 3. Create graphics pipeline ------------------------
		// (lots of boilerplate omitted — you know the drill)
		// fill VkGraphicsPipelineCreateInfo, set viewport, raster state, etc.
		VkGraphicsPipelineCreateInfo pipelineInfo{};

		vkCreateGraphicsPipelines(core.device.device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &rasterPipeline.pipeline);

		return true;
	}

	bool PipelineSystem::CreateCompute(const PipelineConfig& config, VulkanCoreContext& core, const DescriptorSystem& desc)
	{
		computePipeline.computeShader =
			LoadShaderToComputePipeline(config.computeShaderName.c_str(),
				shaderc_compute_shader,
				core, *this);

		VkPipelineLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		layoutInfo.setLayoutCount = 1;
		layoutInfo.pSetLayouts = &desc.layout;

		vkCreatePipelineLayout(core.device.device, &layoutInfo, nullptr, &computePipeline.layout);

		VkComputePipelineCreateInfo computeInfo{};
		computeInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
		computeInfo.stage = {/* compute shader stage info */ };
		computeInfo.layout = computePipeline.layout;

		vkCreateComputePipelines(core.device.device, VK_NULL_HANDLE, 1, &computeInfo, nullptr, &computePipeline.pipeline);

		return true;
	}

	/// TODO: Implement shader reloading
	bool PipelineSystem::ReloadShaders(const std::string& pipelineName)
	{
		return false;
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

	VkShaderModule PipelineSystem::LoadShaderToComputePipeline(const char* path, shaderc_shader_kind kind, VulkanCoreContext& core, PipelineSystem& pipeline)
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

	/// TODO: Implement pipeline config loading
	PipelineConfig PipelineSystem::LoadPipelineConfig(const std::string& jsonFile)
	{
		return PipelineConfig();
	}

	/// TODO: Implement particle initialization
	bool ParticleModule::InitParticles(const VkPhysicalDevice& physicalDevice, const VkDevice& device)
	{
		maxParticles = 10000;

		VkDeviceSize size = maxParticles * sizeof(Particle);

		// Create storage buffer
		GvkHelper::create_buffer(physicalDevice, device, size,
			VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			&storage.buffer,
			&storage.memory);

		// Create struct for compute pipeline
		// Descriptor set layout, compute shader, etc.

		return true;
	}

	bool ParticleModule::DispatchParticleCompute(VkCommandBuffer& cmd)
	{
		// Bind pipeline
		vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, compute.pipeline);

		// Bind descriptor set
		vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_COMPUTE,
			compute.layout, 0, 1, &compute.descriptorSet, 0, nullptr);

		uint32_t groupCount = (maxParticles + 63) / 64;
		vkCmdDispatch(cmd, groupCount, 1, 1);

		return true;
	}

	/// TODO: Implement particle reset
	bool ParticleModule::ResetParticles()
	{
		return true;
	}

	/// TODO: Implement camera update
	bool RenderFrameState::UpdateCamera()
	{

		return true;
	}

	bool RenderFrameState::BeginFrame()
	{
		// Acquire swapchain image, begin command buffer etc.
		return true;
	}

	bool RenderFrameState::EndFrame()
	{
		// Submit + present logic
		return true;
	}
}
#pragma once
// minimalistic code to draw a single triangle, this is not part of the API.
#include "shaderc/shaderc.h" // needed for compiling shaders at runtime
#ifdef _WIN32 // must use MT platform DLL libraries on windows
#pragma comment(lib, "shaderc_combined.lib") 
#endif
#include "UTIL/FileIntoString.h"
#include "Camera/FreeLookCamera.h"

void PrintLabeledDebugString(const char* label, const char* toPrint)
{
	std::cout << label << toPrint << std::endl;

	//OutputDebugStringA is a windows-only function 
#if defined WIN32 
	OutputDebugStringA(label);
	OutputDebugStringA(toPrint);
#endif
}
class Renderer
{
	// proxy handles
	GW::SYSTEM::GWindow win;
	GW::GRAPHICS::GVulkanSurface vlk;
	VkRenderPass renderPass;
	GW::CORE::GEventReceiver shutdown;

	// what we need at a minimum to draw a triangle
	VkDevice device = nullptr;
	VkPhysicalDevice physicalDevice = nullptr;
	VkBuffer vertexHandle = nullptr;
	VkDeviceMemory vertexData = nullptr;
	VkShaderModule vertexShader = nullptr;
	VkShaderModule fragmentShader = nullptr;
	VkPipeline pipeline = nullptr;
	VkPipelineLayout pipelineLayout = nullptr;

	unsigned int windowWidth, windowHeight;

	// TODO: Part 1c
	struct Vertex {
		float x, y, z, w;
		Vertex(float _x, float _y, float _z, float _w) {
			x = _x;
			y = _y;
			z = _z;
			w = _w;
		}
	};

	// TODO: Part 2a
	GW::MATH::GMatrix matrixProxy;
	GW::MATH::GMATRIXF worldMatrixOne;
	GW::MATH::GMATRIXF worldMatrixTwo;
	GW::MATH::GMATRIXF worldMatrixThree;
	GW::MATH::GMATRIXF worldMatrixFour;
	GW::MATH::GMATRIXF worldMatrixFive;
	GW::MATH::GMATRIXF worldMatrixSix;
	// TODO: Part 2b
	struct SHADER_VARS {
		GW::MATH::GMATRIXF myWorldMatrix[6];
		GW::MATH::GMATRIXF myViewMatrix;
		GW::MATH::GMATRIXF myProjectionMatrix;
	};
	SHADER_VARS instanceData;
	// TODO: Part 3a
	GW::MATH::GMATRIXF viewMatrix;
		// TODO: Part 3f 
	// TODO: Part 2c // TODO: Part 4y
	std::vector<VkBuffer> uniformBufferhandle;
	std::vector<VkDeviceMemory> uniformBufferData;
	unsigned int max_frames;
	// TODO: Part 2e
	VkDescriptorSetLayout uniformDescriptorLayout = nullptr;
	// TODO: Part 2f
	VkDescriptorPool uniformDescriptorPool = nullptr;
	std::vector<VkDescriptorSet> uniformDescriptorSet;
	// TODO: Part 2g
	// TODO: Part 3c
	GW::MATH::GMATRIXF leftHandProjMatrix;
	// TODO: Part 3d
	// TODO: Part 4a


public:
	Renderer(GW::SYSTEM::GWindow _win, GW::GRAPHICS::GVulkanSurface _vlk)
	{
		win = _win;
		vlk = _vlk;

		// TODO: Part 2a
		matrixProxy.Create();
		
		InitializeWorldMatrixOne_FloorGrid();
		InitializeWorldMatrixTwo_CeilingGrid();
		InitializeWorldMatrixThree_BackLeftWall();
		InitializeWorldMatrixFour_BackRightWall();
		InitializeWorldMatrixFive_FrontRightWall();
		InitializeWorldMatrixSix_FrontLeftWall();

		// TODO: Part 2e
		GetHandlesFromSurface();
		InitializeDescriptorLayout();

		// TODO: Part 3a
		InitializeViewMatrix(0.0f, -1.5f, -1.0f, 45.0f, 0.0f);
		// TODO: Part 3c
		float aspectRatio = 0.0f;
		vlk.GetAspectRatio(aspectRatio);
		InitializeProjectionMatrix(0.1f, 100.0f, 65.0f, aspectRatio);
		// TODO: Part 3d
		// TODO: Part 4a

		UpdateWindowDimensions();
		InitializeGraphics();
		BindShutdownCallback();
	}

	void InitializeProjectionMatrix(float nearPlane, float farPlane, float verticalFOV, float aspectRatio) {
		matrixProxy.IdentityF(leftHandProjMatrix);
		matrixProxy.IdentityF(instanceData.myProjectionMatrix);
		matrixProxy.ProjectionVulkanLHF(verticalFOV, aspectRatio, nearPlane, farPlane, leftHandProjMatrix);
	}

	void InitializeViewMatrix(float xDistance, float yDistance, float zDistance, float xRotationDegrees, float yRotationDegrees) {
		// initialize view matrix as identity
		matrixProxy.IdentityF(instanceData.myViewMatrix);
		matrixProxy.IdentityF(viewMatrix);
		// initialize translation matrix as identity
		GW::MATH::GMATRIXF translationAdj;
		matrixProxy.IdentityF(translationAdj);
		// initialize x-rot matrix as identity
		GW::MATH::GMATRIXF xRotAdj;
		matrixProxy.IdentityF(xRotAdj);
		// initialize y-rot matrix as identity
		GW::MATH::GMATRIXF yRotAdj;
		matrixProxy.IdentityF(yRotAdj);


		// translation matrix
		GW::MATH::GVECTORF tempVec = {};
		tempVec.x = xDistance;
		tempVec.y = yDistance;
		tempVec.z = zDistance;
		tempVec.w = 1.0f;
		matrixProxy.TranslateGlobalF(translationAdj, tempVec, translationAdj);

		// x-axis rotation matrix
		float x_rads = xRotationDegrees * (3.14f / 180.0f);
		matrixProxy.RotateXGlobalF(xRotAdj, x_rads, xRotAdj);

		// y-axis rotation matrix
		float y_rads = yRotationDegrees * (3.14f / 180.0f);
		matrixProxy.RotateYGlobalF(yRotAdj, y_rads, yRotAdj);

		// matrix multiplications
		GW::MATH::GMATRIXF tempMatrix;
		matrixProxy.IdentityF(tempMatrix);

		matrixProxy.MultiplyMatrixF(translationAdj, tempMatrix, tempMatrix);
		matrixProxy.MultiplyMatrixF(tempMatrix, xRotAdj, tempMatrix);
		matrixProxy.MultiplyMatrixF(tempMatrix, yRotAdj, tempMatrix);
		
		// inverse the view for the camera
		matrixProxy.InverseF(tempMatrix, viewMatrix);
	}

	void InitializeWorldMatrixOne_FloorGrid() {
		matrixProxy.IdentityF(instanceData.myWorldMatrix[0]);
		
		// 90 degree rotation
		float rads = 90 *(3.14f / 180.0f);
		GW::MATH::GMATRIXF firstAdj;
		matrixProxy.IdentityF(firstAdj);
		matrixProxy.RotateXGlobalF(firstAdj, rads, firstAdj);
		// -0.5f translation on y
		GW::MATH::GVECTORF temp = {};
		temp.x = 0.0f;
		temp.y = -0.5f;
		temp.z = 0.0f;
		temp.w = 1.0f;
		GW::MATH::GMATRIXF secAdj;
		matrixProxy.IdentityF(secAdj);
		matrixProxy.TranslateGlobalF(secAdj, temp, secAdj);
		matrixProxy.MultiplyMatrixF(firstAdj, secAdj, worldMatrixOne);
	}

	void InitializeWorldMatrixTwo_CeilingGrid() {
		matrixProxy.IdentityF(instanceData.myWorldMatrix[1]);

		float rads = 90 * (3.14f / 180.0f);
		GW::MATH::GMATRIXF firstAdj;
		matrixProxy.IdentityF(firstAdj);
		matrixProxy.RotateXGlobalF(firstAdj, rads, firstAdj);


		GW::MATH::GVECTORF temp = {};
		temp.x = 0.0f;
		temp.y = -1.5f;
		temp.z = 0.0f;
		temp.w = 1.0f;
		GW::MATH::GMATRIXF secAdj;
		matrixProxy.IdentityF(secAdj);
		matrixProxy.TranslateGlobalF(secAdj, temp, secAdj);
		matrixProxy.MultiplyMatrixF(firstAdj, secAdj, worldMatrixTwo);
	}

	void InitializeWorldMatrixThree_BackLeftWall() {
		matrixProxy.IdentityF(instanceData.myWorldMatrix[2]);
		
		// zero rotation on the x-axis
		float rads = 180 * (3.14f / 180.0f);
		GW::MATH::GMATRIXF firstAdj;
		matrixProxy.IdentityF(firstAdj);
		matrixProxy.RotateXLocalF(firstAdj, rads, firstAdj);


		GW::MATH::GVECTORF temp = {};
		temp.x = 0.0f;
		temp.y = -1.0f;
		temp.z = 0.5f;
		temp.w = 1.0f;
		GW::MATH::GMATRIXF secAdj;
		matrixProxy.IdentityF(secAdj);
		matrixProxy.TranslateGlobalF(secAdj, temp, secAdj);
		matrixProxy.MultiplyMatrixF(firstAdj, secAdj, worldMatrixThree);
	}

	void InitializeWorldMatrixFour_BackRightWall() {
		matrixProxy.IdentityF(instanceData.myWorldMatrix[3]);

		// zero rotation on the x-axis
		float rads = 90 * (3.14f / 180.0f);
		GW::MATH::GMATRIXF firstAdj;
		matrixProxy.IdentityF(firstAdj);
		matrixProxy.RotateYLocalF(firstAdj, rads, firstAdj);


		GW::MATH::GVECTORF temp = {};
		temp.x = 0.5f;
		temp.y = -1.0f;
		temp.z = 0.0f;
		temp.w = 1.0f;
		GW::MATH::GMATRIXF secAdj;
		matrixProxy.IdentityF(secAdj);
		matrixProxy.TranslateGlobalF(secAdj, temp, secAdj);
		matrixProxy.MultiplyMatrixF(firstAdj, secAdj, worldMatrixFour);
	}

	void InitializeWorldMatrixFive_FrontRightWall() {
		matrixProxy.IdentityF(instanceData.myWorldMatrix[4]);

		// zero rotation on the x-axis
		float rads = 180 * (3.14f / 180.0f);
		GW::MATH::GMATRIXF firstAdj;
		matrixProxy.IdentityF(firstAdj);
		matrixProxy.RotateXLocalF(firstAdj, rads, firstAdj);


		GW::MATH::GVECTORF temp = {};
		temp.x = 0.0f;
		temp.y = -1.0f;
		temp.z = -0.5f;
		temp.w = 1.0f;
		GW::MATH::GMATRIXF secAdj;
		matrixProxy.IdentityF(secAdj);
		matrixProxy.TranslateGlobalF(secAdj, temp, secAdj);
		matrixProxy.MultiplyMatrixF(firstAdj, secAdj, worldMatrixFive);
	}

	void InitializeWorldMatrixSix_FrontLeftWall() {
		matrixProxy.IdentityF(instanceData.myWorldMatrix[5]);

		// zero rotation on the x-axis
		float rads = 90 * (3.14f / 180.0f);
		GW::MATH::GMATRIXF firstAdj;
		matrixProxy.IdentityF(firstAdj);
		matrixProxy.RotateYLocalF(firstAdj, rads, firstAdj);


		GW::MATH::GVECTORF temp = {};
		temp.x = -0.5f;
		temp.y = -1.0f;
		temp.z = 0.0f;
		temp.w = 1.0f;
		GW::MATH::GMATRIXF secAdj;
		matrixProxy.IdentityF(secAdj);
		matrixProxy.TranslateGlobalF(secAdj, temp, secAdj);
		matrixProxy.MultiplyMatrixF(firstAdj, secAdj, worldMatrixSix);
	}


	// TODO: Part 2e
	VkDescriptorSetLayoutBinding InitializeDescriptorLayoutBindings() {
		VkDescriptorSetLayoutBinding retval = {};
		retval.binding = 0;
		retval.descriptorCount = 1;
		retval.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		retval.pImmutableSamplers = nullptr;
		retval.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		return retval;
	}
	// TODO: Part 2e
	VkDescriptorSetLayoutCreateInfo InitializeDescriptorLayoutCreateInfo(VkDescriptorSetLayoutBinding& layoutBinding, int bindingCount) {
		VkDescriptorSetLayoutCreateInfo retval = {};
		retval.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		retval.pNext = nullptr;
		retval.bindingCount = bindingCount;
		retval.flags = 0;
		retval.pBindings = &layoutBinding;
		return retval;
	}
	// TODO: Part 2e
	void InitializeDescriptorLayout() {
		VkDescriptorSetLayoutBinding uniformDescriptorLayoutBinding = InitializeDescriptorLayoutBindings();
		VkDescriptorSetLayoutCreateInfo uniformDescriptorLayoutCreateInfo = InitializeDescriptorLayoutCreateInfo(uniformDescriptorLayoutBinding, 1);
		vkCreateDescriptorSetLayout(device, &uniformDescriptorLayoutCreateInfo, nullptr, &uniformDescriptorLayout);
	}

private:
	void UpdateWindowDimensions()
	{
		win.GetClientWidth(windowWidth);
		win.GetClientHeight(windowHeight);
	}

	void InitializeGraphics()
	{
		GetHandlesFromSurface();
		InitializeVertexBuffer();
		// TODO: Part 2d
		InitializeUniformBuffers();
		// TODO: Part 2f // TODO: Part 4y
		InitializeDescriptorPool();
		// TODO: Part 2g // TODO: Part 4y
		InitializeDescriptorSets();
		// TODO: Part 2h // TODO: Part 4y
		LinkDescriptorsToUniformBuffer();


		CompileShaders();
		InitializeGraphicsPipeline();
	}
	// TODO: Part 2f
	void InitializeDescriptorPool() {
		VkDescriptorPoolCreateInfo poolCreateInfo = InitializeDescriptorPoolCreateInfo();
		vkCreateDescriptorPool(device, &poolCreateInfo, nullptr, &uniformDescriptorPool);
	}
	// TODO: Part 2f
	VkDescriptorPoolCreateInfo InitializeDescriptorPoolCreateInfo() {
		VkDescriptorPoolCreateInfo retval = {};
		retval.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		VkDescriptorPoolSize descriptorpool_size = { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, max_frames };
		retval.poolSizeCount = 1;
		retval.pPoolSizes = &descriptorpool_size;
		retval.maxSets = max_frames;
		retval.flags = 0;
		retval.pNext = nullptr;
		return retval;
	}
	// TODO: Part 2g
	VkDescriptorSetAllocateInfo InitializeDescriptorSetAllocateInfo() {
		VkDescriptorSetAllocateInfo retval = {};
		retval.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		retval.pNext = nullptr;
		retval.descriptorPool = uniformDescriptorPool;
		retval.descriptorSetCount = 1;
		retval.pSetLayouts = &uniformDescriptorLayout;
		return retval;
	}
	// TODO: Part 2g
	void InitializeDescriptorSets() {
		VkDescriptorSetAllocateInfo allocateInfo = InitializeDescriptorSetAllocateInfo();
		uniformDescriptorSet.resize(max_frames);
		for (int i = 0; i < max_frames; i++) {
			vkAllocateDescriptorSets(device, &allocateInfo, &uniformDescriptorSet[i]);
		}
	}
	// TODO: Part 2h
	void LinkDescriptorsToUniformBuffer() {
		VkWriteDescriptorSet writeDescriptor = InitializeWriteDescriptorSet();
		for (int i = 0; i < max_frames; i++) {
			writeDescriptor.dstSet = uniformDescriptorSet[i];
			VkDescriptorBufferInfo bufferInfo = { uniformBufferhandle[i], 0, VK_WHOLE_SIZE };
			writeDescriptor.pBufferInfo = &bufferInfo;
			vkUpdateDescriptorSets(device, 1, &writeDescriptor, 0, nullptr);
		}
	}

	VkWriteDescriptorSet InitializeWriteDescriptorSet() {
		VkWriteDescriptorSet retval = {};
		retval.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		retval.descriptorCount = 1;
		retval.dstArrayElement = 0;
		retval.dstBinding = 0;
		retval.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		return retval;
	}

	void InitializeUniformBuffers() {
		max_frames = 0;
		vlk.GetSwapchainImageCount(max_frames);
		uniformBufferhandle.resize(max_frames);
		uniformBufferData.resize(max_frames);

		//instanceData.myWorldMatrix[0] = worldMatrixOne;
		//instanceData.myWorldMatrix[1] = worldMatrixTwo;
		//instanceData.myWorldMatrix[2] = worldMatrixThree;
		//instanceData.myWorldMatrix[3] = worldMatrixFour;
		//instanceData.myWorldMatrix[4] = worldMatrixFive;
		//instanceData.myWorldMatrix[5] = worldMatrixSix;
		//instanceData.myViewMatrix = viewMatrix;
		//instanceData.myProjectionMatrix = leftHandProjMatrix;

		for (int i = 0; i < max_frames; i++) {
			GvkHelper::create_buffer(physicalDevice, device, sizeof(SHADER_VARS), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				&uniformBufferhandle[i], &uniformBufferData[i]);
			GvkHelper::write_to_buffer(device, uniformBufferData[i], &instanceData, sizeof(SHADER_VARS));
		}
	}

	void GetHandlesFromSurface()
	{
		vlk.GetDevice((void**)&device);
		vlk.GetPhysicalDevice((void**)&physicalDevice);
		vlk.GetRenderPass((void**)&renderPass);
	}

	void InitializeVertexBuffer()
	{
		// TODO: Part 1b
		{
			//float verts[] = 
			//{
			//	0.0f,   0.5f,		// right line
			//	0.5f, -0.5f,

			//	0.5f, -0.5f,		// bottom line
			//	-0.5f, -0.5f,

			//	-0.5f, -0.5f,		// left line
			//	0.0f, 0.5f,
			//};
			//CreateVertexBuffer(&verts[0], sizeof(verts));
		}


		// TODO: Part 1c
		//Vertex verts[] =
		//{
		//	Vertex(0.0f, 0.5f, 0.0f, 1.0f),			// right line
		//	Vertex(0.5f, -0.5f, 0.0f, 1.0f),

		//	Vertex(0.5f, -0.5f, 0.0f, 1.0f),		// bottom line
		//	Vertex(-0.5f, -0.5f, 0.0f, 1.0f),

		//	Vertex(-0.5f, -0.5f, 0.0f, 1.0f),		// left line
		//	Vertex(0.0f, 0.5f, 0.0f, 1.0f)
		//};
		//CreateVertexBuffer(&verts[0], sizeof(verts));

		// TODO: Part 1d
		std::vector<Vertex> gridSegment;
		gridSegment.clear();
		float width = 0.5f;
		BuildSquareGridOfVertices(gridSegment, 26, width);

		CreateVertexBuffer(&gridSegment[0], sizeof(std::vector<Vertex>) * gridSegment.size());

	}
	
	// TODO: Part 1d helper function
	void BuildSquareGridOfVertices(std::vector<Vertex>& toReturn, int dimensions, float width = 0.5f) {
		for (int i = 0; i < dimensions; i++) {
			float gap = (float)(i * (1.0f / (dimensions - 1)) - width);
			toReturn.push_back(Vertex(gap, -width, 0.0f, 1.0f));
			toReturn.push_back(Vertex(gap, width, 0.0f, 1.0f));
			toReturn.push_back(Vertex(-width, gap, 0.0f, 1.0f));
			toReturn.push_back(Vertex(width, gap, 0.0f, 1.0f));
		}
	}

	void CreateVertexBuffer(const void* data, unsigned int sizeInBytes)
	{
		GvkHelper::create_buffer(physicalDevice, device, sizeInBytes, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			&vertexHandle, &vertexData);
		GvkHelper::write_to_buffer(device, vertexData, data, sizeInBytes); // Transfer triangle data to the vertex buffer. (staging would be prefered here)
	}

	void CompileShaders()
	{
		// Intialize runtime shader compiler HLSL -> SPIRV
		shaderc_compiler_t compiler = shaderc_compiler_initialize();
		shaderc_compile_options_t options = CreateCompileOptions();

		CompileVertexShader(compiler, options);
		CompileFragmentShader(compiler, options);

		// Free runtime shader compiler resources
		shaderc_compile_options_release(options);
		shaderc_compiler_release(compiler);
	}

	shaderc_compile_options_t CreateCompileOptions()
	{
		shaderc_compile_options_t retval = shaderc_compile_options_initialize();
		shaderc_compile_options_set_source_language(retval, shaderc_source_language_hlsl);
		shaderc_compile_options_set_invert_y(retval, true);	// TODO: Part 3e
#ifndef NDEBUG
		shaderc_compile_options_set_generate_debug_info(retval);
#endif
		return retval;
	}

	void CompileVertexShader(const shaderc_compiler_t& compiler, const shaderc_compile_options_t& options)
	{
		std::string vertexShaderSource = ReadFileIntoString("../Shaders/VertexShader.hlsl");

		shaderc_compilation_result_t result = shaderc_compile_into_spv( // compile
			compiler, vertexShaderSource.c_str(), vertexShaderSource.length(),
			shaderc_vertex_shader, "main.vert", "main", options);

		if (shaderc_result_get_compilation_status(result) != shaderc_compilation_status_success) // errors?
		{
			PrintLabeledDebugString("Vertex Shader Errors: \n", shaderc_result_get_error_message(result));
			abort(); //Vertex shader failed to compile! 
			return;
		}

		GvkHelper::create_shader_module(device, shaderc_result_get_length(result), // load into Vulkan
			(char*)shaderc_result_get_bytes(result), &vertexShader);

		shaderc_result_release(result); // done
	}

	void CompileFragmentShader(const shaderc_compiler_t& compiler, const shaderc_compile_options_t& options)
	{
		std::string fragmentShaderSource = ReadFileIntoString("../Shaders/FragmentShader.hlsl");

		shaderc_compilation_result_t result = shaderc_compile_into_spv( // compile
			compiler, fragmentShaderSource.c_str(), fragmentShaderSource.length(),
			shaderc_fragment_shader, "main.frag", "main", options);

		if (shaderc_result_get_compilation_status(result) != shaderc_compilation_status_success) // errors?
		{
			PrintLabeledDebugString("Fragment Shader Errors: \n", shaderc_result_get_error_message(result));
			abort(); //Fragment shader failed to compile! 
			return;
		}

		GvkHelper::create_shader_module(device, shaderc_result_get_length(result), // load into Vulkan
			(char*)shaderc_result_get_bytes(result), &fragmentShader);

		shaderc_result_release(result); // done
	}

	// Create Pipeline & Layout (Thanks Tiny!)
	void InitializeGraphicsPipeline()
	{
		VkPipelineShaderStageCreateInfo stage_create_info[2] = {};

		// Create Stage Info for Vertex Shader
		stage_create_info[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		stage_create_info[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		stage_create_info[0].module = vertexShader;
		stage_create_info[0].pName = "main";

		// Create Stage Info for Fragment Shader
		stage_create_info[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		stage_create_info[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		stage_create_info[1].module = fragmentShader;
		stage_create_info[1].pName = "main";


		VkPipelineInputAssemblyStateCreateInfo assembly_create_info = CreateVkPipelineInputAssemblyStateCreateInfo();
		VkVertexInputBindingDescription vertex_binding_description = CreateVkVertexInputBindingDescription();

		// TODO: Part 1c
		VkVertexInputAttributeDescription vertex_attribute_descriptions[1];
		vertex_attribute_descriptions[0].binding = 0;
		vertex_attribute_descriptions[0].location = 0;
		vertex_attribute_descriptions[0].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		vertex_attribute_descriptions[0].offset = 0;

		VkPipelineVertexInputStateCreateInfo input_vertex_info = CreateVkPipelineVertexInputStateCreateInfo(&vertex_binding_description, 1, vertex_attribute_descriptions, 1);
		VkViewport viewport = CreateViewportFromWindowDimensions();
		VkRect2D scissor = CreateScissorFromWindowDimensions();
		VkPipelineViewportStateCreateInfo viewport_create_info = CreateVkPipelineViewportStateCreateInfo(&viewport, 1, &scissor, 1);
		VkPipelineRasterizationStateCreateInfo rasterization_create_info = CreateVkPipelineRasterizationStateCreateInfo();
		VkPipelineMultisampleStateCreateInfo multisample_create_info = CreateVkPipelineMultisampleStateCreateInfo();
		VkPipelineDepthStencilStateCreateInfo depth_stencil_create_info = CreateVkPipelineDepthStencilStateCreateInfo();
		VkPipelineColorBlendAttachmentState color_blend_attachment_state = CreateVkPipelineColorBlendAttachmentState();
		VkPipelineColorBlendStateCreateInfo color_blend_create_info = CreateVkPipelineColorBlendStateCreateInfo(&color_blend_attachment_state, 1);

		VkDynamicState dynamic_states[2] =
		{
			// By setting these we do not need to re-create the pipeline on Resize
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		VkPipelineDynamicStateCreateInfo dynamic_create_info = CreateVkPipelineDynamicStateCreateInfo(dynamic_states, 2);

		CreatePipelineLayout();

		// Pipeline State... (FINALLY) 
		VkGraphicsPipelineCreateInfo pipeline_create_info = {};
		pipeline_create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipeline_create_info.stageCount = 2;
		pipeline_create_info.pStages = stage_create_info;
		pipeline_create_info.pInputAssemblyState = &assembly_create_info;
		pipeline_create_info.pVertexInputState = &input_vertex_info;
		pipeline_create_info.pViewportState = &viewport_create_info;
		pipeline_create_info.pRasterizationState = &rasterization_create_info;
		pipeline_create_info.pMultisampleState = &multisample_create_info;
		pipeline_create_info.pDepthStencilState = &depth_stencil_create_info;
		pipeline_create_info.pColorBlendState = &color_blend_create_info;
		pipeline_create_info.pDynamicState = &dynamic_create_info;
		pipeline_create_info.layout = pipelineLayout;
		pipeline_create_info.renderPass = renderPass;
		pipeline_create_info.subpass = 0;
		pipeline_create_info.basePipelineHandle = VK_NULL_HANDLE;

		vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipeline_create_info, nullptr, &pipeline);
	}

	VkPipelineShaderStageCreateInfo CreateVertexShaderStageCreateInfo()
	{
		VkPipelineShaderStageCreateInfo retval;
		retval.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		retval.stage = VK_SHADER_STAGE_VERTEX_BIT;
		retval.module = vertexShader;
		retval.pName = "main";
		return retval;
	}

	VkPipelineInputAssemblyStateCreateInfo CreateVkPipelineInputAssemblyStateCreateInfo()
	{
		VkPipelineInputAssemblyStateCreateInfo retval = {};
		retval.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		retval.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST; // TODO: Part 1b
		retval.primitiveRestartEnable = false;
		return retval;
	}

	VkVertexInputBindingDescription CreateVkVertexInputBindingDescription()
	{
		VkVertexInputBindingDescription retval = {};
		retval.binding = 0;
		retval.stride = sizeof(Vertex); 	//TODO: Part 1c
		retval.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return retval;
	}

	VkPipelineVertexInputStateCreateInfo CreateVkPipelineVertexInputStateCreateInfo(
		VkVertexInputBindingDescription* inputBindingDescriptions, unsigned int bindingCount,
		VkVertexInputAttributeDescription* vertexAttributeDescriptions, unsigned int attributeCount)
	{
		VkPipelineVertexInputStateCreateInfo retval = {};
		retval.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		retval.vertexBindingDescriptionCount = bindingCount;
		retval.pVertexBindingDescriptions = inputBindingDescriptions;
		retval.vertexAttributeDescriptionCount = attributeCount;
		retval.pVertexAttributeDescriptions = vertexAttributeDescriptions;
		return retval;
	}

	VkViewport CreateViewportFromWindowDimensions()
	{
		VkViewport retval = {};
		retval.x = 0;
		retval.y = 0;
		retval.width = static_cast<float>(windowWidth);
		retval.height = static_cast<float>(windowHeight);
		retval.minDepth = 0;
		retval.maxDepth = 1;
		return retval;
	}

	VkRect2D CreateScissorFromWindowDimensions()
	{
		VkRect2D retval = {};
		retval.offset.x = 0;
		retval.offset.y = 0;
		retval.extent.width = windowWidth;
		retval.extent.height = windowHeight;
		return retval;
	}

	VkPipelineViewportStateCreateInfo CreateVkPipelineViewportStateCreateInfo(VkViewport* viewports, unsigned int viewportCount, VkRect2D* scissors, unsigned int scissorCount)
	{
		VkPipelineViewportStateCreateInfo retval = {};
		retval.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		retval.viewportCount = viewportCount;
		retval.pViewports = viewports;
		retval.scissorCount = scissorCount;
		retval.pScissors = scissors;
		return retval;
	}

	VkPipelineRasterizationStateCreateInfo CreateVkPipelineRasterizationStateCreateInfo()
	{
		VkPipelineRasterizationStateCreateInfo retval = {};
		retval.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		retval.rasterizerDiscardEnable = VK_FALSE;
		retval.polygonMode = VK_POLYGON_MODE_FILL;
		retval.lineWidth = 1.0f;
		retval.cullMode = VK_CULL_MODE_BACK_BIT;
		retval.frontFace = VK_FRONT_FACE_CLOCKWISE;
		retval.depthClampEnable = VK_FALSE;
		retval.depthBiasEnable = VK_FALSE;
		retval.depthBiasClamp = 0.0f;
		retval.depthBiasConstantFactor = 0.0f;
		retval.depthBiasSlopeFactor = 0.0f;
		return retval;
	}

	VkPipelineMultisampleStateCreateInfo CreateVkPipelineMultisampleStateCreateInfo()
	{
		VkPipelineMultisampleStateCreateInfo retval = {};
		retval.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		retval.sampleShadingEnable = VK_FALSE;
		retval.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		retval.minSampleShading = 1.0f;
		retval.pSampleMask = VK_NULL_HANDLE;
		retval.alphaToCoverageEnable = VK_FALSE;
		retval.alphaToOneEnable = VK_FALSE;
		return retval;
	}

	VkPipelineDepthStencilStateCreateInfo CreateVkPipelineDepthStencilStateCreateInfo()
	{
		VkPipelineDepthStencilStateCreateInfo retval = {};
		retval.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		retval.depthTestEnable = VK_TRUE;
		retval.depthWriteEnable = VK_TRUE;
		retval.depthCompareOp = VK_COMPARE_OP_LESS;
		retval.depthBoundsTestEnable = VK_FALSE;
		retval.minDepthBounds = 0.0f;
		retval.maxDepthBounds = 1.0f;
		retval.stencilTestEnable = VK_FALSE;
		return retval;
	}

	VkPipelineColorBlendAttachmentState CreateVkPipelineColorBlendAttachmentState()
	{
		VkPipelineColorBlendAttachmentState retval = {};
		retval.colorWriteMask = 0xF;
		retval.blendEnable = VK_FALSE;
		retval.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_COLOR;
		retval.dstColorBlendFactor = VK_BLEND_FACTOR_DST_COLOR;
		retval.colorBlendOp = VK_BLEND_OP_ADD;
		retval.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		retval.dstAlphaBlendFactor = VK_BLEND_FACTOR_DST_ALPHA;
		retval.alphaBlendOp = VK_BLEND_OP_ADD;
		return retval;
	}

	VkPipelineColorBlendStateCreateInfo CreateVkPipelineColorBlendStateCreateInfo(VkPipelineColorBlendAttachmentState* attachments, unsigned int attachmentCount)
	{
		VkPipelineColorBlendStateCreateInfo retval = {};
		retval.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		retval.logicOpEnable = VK_FALSE;
		retval.logicOp = VK_LOGIC_OP_COPY;
		retval.attachmentCount = attachmentCount;
		retval.pAttachments = attachments;
		retval.blendConstants[0] = 0.0f;
		retval.blendConstants[1] = 0.0f;
		retval.blendConstants[2] = 0.0f;
		retval.blendConstants[3] = 0.0f;
		return retval;
	}

	VkPipelineDynamicStateCreateInfo CreateVkPipelineDynamicStateCreateInfo(VkDynamicState* dynamicStates, unsigned int dynamicStateCount)
	{
		VkPipelineDynamicStateCreateInfo retval = {};
		retval.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		retval.dynamicStateCount = dynamicStateCount;
		retval.pDynamicStates = dynamicStates;
		return retval;
	}

	void CreatePipelineLayout()
	{
		// TODO: Part 2e

		VkPipelineLayoutCreateInfo pipeline_layout_create_info = {};
		pipeline_layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipeline_layout_create_info.setLayoutCount = 1; // TODO: Part 2e
		pipeline_layout_create_info.pSetLayouts = &uniformDescriptorLayout; // TODO: Part 2e
		pipeline_layout_create_info.pushConstantRangeCount = 0;
		pipeline_layout_create_info.pPushConstantRanges = nullptr;

		vkCreatePipelineLayout(device, &pipeline_layout_create_info, nullptr, &pipelineLayout);
	}

	void BindShutdownCallback()
	{
		// GVulkanSurface will inform us when to release any allocated resources
		shutdown.Create(vlk, [&]() {
			if (+shutdown.Find(GW::GRAPHICS::GVulkanSurface::Events::RELEASE_RESOURCES, true)) {
				CleanUp(); // unlike D3D we must be careful about destroy timing
			}
			});
	}


public:
	void Render()
	{
		VkCommandBuffer commandBuffer = GetCurrentCommandBuffer();
		// TODO: Part 4x
		unsigned int currentBuffer = 0;
		vlk.GetSwapchainCurrentImage(currentBuffer);
		//instanceData.myViewMatrix = FreeLookCamera(win, instanceData.myViewMatrix);
		instanceData.myViewMatrix = viewMatrix;
		GvkHelper::write_to_buffer(device, uniformBufferData[currentBuffer], &instanceData, sizeof(SHADER_VARS));

		SetUpPipeline(commandBuffer);

		// TODO: Part 2b
		instanceData.myWorldMatrix[0] = worldMatrixOne;
		instanceData.myWorldMatrix[1] = worldMatrixTwo;
		instanceData.myWorldMatrix[2] = worldMatrixThree;
		instanceData.myWorldMatrix[3] = worldMatrixFour;
		instanceData.myWorldMatrix[4] = worldMatrixFive;
		instanceData.myWorldMatrix[5] = worldMatrixSix;
		instanceData.myViewMatrix = viewMatrix;
		instanceData.myProjectionMatrix = leftHandProjMatrix;
		 
		
		// TODO: Part 2i // TODO: Part 4y
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &uniformDescriptorSet[currentBuffer], 0, nullptr);


		// TODO: Part 3g
		vkCmdDraw(commandBuffer, 104, 6, 0, 0); // TODO: Part 1b 
	}

	// TODO: Part 4b
	// TODO: Part 4c
	// TODO: Part 4d
	// TODO: Part 4e
	// TODO: Part 4f
	// TODO: Part 4g

private:
	VkCommandBuffer GetCurrentCommandBuffer()
	{
		unsigned int currentBuffer;
		vlk.GetSwapchainCurrentImage(currentBuffer);

		VkCommandBuffer commandBuffer;
		vlk.GetCommandBuffer(currentBuffer, (void**)&commandBuffer);
		return commandBuffer;
	}

	void SetUpPipeline(VkCommandBuffer& commandBuffer)
	{
		UpdateWindowDimensions();
		SetViewport(commandBuffer);
		SetScissor(commandBuffer);
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
		BindVertexBuffers(commandBuffer);
	}

	void SetViewport(const VkCommandBuffer& commandBuffer)
	{
		VkViewport viewport = CreateViewportFromWindowDimensions();
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	}

	void SetScissor(const VkCommandBuffer& commandBuffer)
	{
		VkRect2D scissor = CreateScissorFromWindowDimensions();
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	}

	void BindVertexBuffers(VkCommandBuffer& commandBuffer)
	{
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexHandle, offsets);
	}


	//Cleanup callback function (passed to VKSurface, will be called when the pipeline shuts down)
	void CleanUp()
	{
		// wait till everything has completed
		vkDeviceWaitIdle(device);

		// Release allocated buffers, shaders & pipeline
		vkDestroyBuffer(device, vertexHandle, nullptr);
		vkFreeMemory(device, vertexData, nullptr);
		// TODO: Part 2d
		max_frames = 0;
		vlk.GetSwapchainImageCount(max_frames);
		for (int i = 0; i < max_frames; i++) {
			if (uniformBufferhandle[i] != nullptr) {
				vkDestroyBuffer(device, uniformBufferhandle[i], nullptr);
			}
			if (uniformBufferData[i] != nullptr) {
				vkFreeMemory(device, uniformBufferData[i], nullptr);
			}
		}
		uniformBufferhandle.clear();
		uniformBufferData.clear();
		// TODO: Part 2f
		vkDestroyDescriptorSetLayout(device, uniformDescriptorLayout, nullptr);
		vkDestroyDescriptorPool(device, uniformDescriptorPool, nullptr);
		vkDestroyShaderModule(device, vertexShader, nullptr);
		vkDestroyShaderModule(device, fragmentShader, nullptr);
		vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
		vkDestroyPipeline(device, pipeline, nullptr);
	}
};

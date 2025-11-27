#ifndef DRAW_COMPONENTS_H
#define DRAW_COMPONENTS_H

#include "../../Utils/IuvoUtils.h"
#include "../Camera/Camera.h"

namespace DRAW
{
	//======================== TAGS ========================//
	struct DoNotRenderTag {};
	struct ParticleRenderTag {};

	//==================== BASIC STRUCTS ====================//
	//Raster state must be defined before PipelineConfig
	struct RasterState {
		bool enableDepthTest = true;
		bool enableDepthWrite = true;
		bool enableBlending = false;
		bool wireframe = false;
		VkCullModeFlags cullMode = VK_CULL_MODE_BACK_BIT;
	};

	//=================== LOW-LEVEL VULKAN ===================//
	struct VulkanVertexBuffer
	{
		VkBuffer buffer = VK_NULL_HANDLE;
		VkDeviceMemory memory = VK_NULL_HANDLE;
	};

	struct VulkanIndexBuffer
	{
		VkBuffer buffer = VK_NULL_HANDLE;
		VkDeviceMemory memory = VK_NULL_HANDLE;
	};

	struct VulkanBuffer {
		VulkanVertexBuffer vertexBuffer;
		VulkanIndexBuffer indexBuffer;
	};

	struct VulkanUniformBuffer {
		std::vector<VkBuffer> buffer;
		std::vector<VkDeviceMemory> memory;
		uint32_t alignedSize = 0;
		uint32_t bufferCount = 0;
	};

	struct VulkanInstanceBuffer {
		unsigned long long element_count = 1;
		std::vector<VkBuffer> buffer;
		std::vector<VkDeviceMemory> memory;
	};

	struct VulkanParticleStorage {
		VkBuffer buffer;
		VkDeviceMemory memory;
		uint32_t count = 0;
	};

	struct VulkanTexture {
		VkImage image = VK_NULL_HANDLE;
		VkDeviceMemory memory = VK_NULL_HANDLE;
		VkImageView view = VK_NULL_HANDLE;
		VkSampler sampler = VK_NULL_HANDLE;
	};

	//==================== CORE VULKAN =======================//
	struct DeviceResources
	{
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkDevice device = VK_NULL_HANDLE;
		VkQueue graphicsQueue = VK_NULL_HANDLE;
		uint32_t graphicsQueueFamily = 0;
		VkCommandPool commandPool = VK_NULL_HANDLE;
	};

	struct SwapchainResources
	{
		VkSwapchainKHR swapchain = VK_NULL_HANDLE;
		uint32_t minImageCount = 2;
		uint32_t actualImageCount = 0;
		std::vector<VkImage> images;
		std::vector<VkImageView> imageViews;
		VkFormat imageFormat = VK_FORMAT_UNDEFINED;
		VkExtent2D extent = { 0, 0 };
		std::vector<VkFramebuffer> framebuffers;
	};

	struct FrameSync
	{
		VkSemaphore imageAvailable = VK_NULL_HANDLE;
		VkSemaphore renderFinished = VK_NULL_HANDLE;
		VkFence inFlight = VK_NULL_HANDLE;
	};

	struct RenderPassResources
	{
		VkRenderPass renderPass = VK_NULL_HANDLE;
		VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
		VkClearValue clearValues[2] =
		{
			{0.2f, 0.2f, 0.2f, 1.0f}, // Color clear
			{ 0.75f, 0 }              // Depth clear
		};

	};

	struct VulkanCoreContext {
		DeviceResources device;
		SwapchainResources swapchain;
		RenderPassResources renderPass;
		std::vector<FrameSync> syncFrames;
		std::vector<VkCommandBuffer> commandBuffers;

		bool InitDevice(VkInstance);
		bool InitSwapchain();
		bool InitRenderPass();
		bool InitCommandBuffers();
		VkCommandBuffer BeginSingleUseCmd();
		void EndSingleUseCmd(VkCommandBuffer cmd);
	};

	//================ RESOURCE MANAGEMENT ==================//
	struct TextureManager {
		std::vector<VulkanTexture> textures;
		VulkanTexture CreateTextureFromFile(const std::string& path);
		VulkanTexture CreateTextureFromMemory();
	};

	struct BufferManager {
		std::vector<VulkanBuffer> geometry;
		VulkanBuffer CreateVertexIndexBuffer(const VkPhysicalDevice& physicalDevice, const VkDevice& device, VkDeviceSize& vertSize, VkDeviceSize& indexSize);
		VulkanUniformBuffer CreateUniformBuffers(size_t size, uint32_t count);
		VulkanInstanceBuffer CreateInstanceBuffer(uint32_t count);
	};

	struct DescriptorSystem {
		VkDescriptorSetLayout layout = VK_NULL_HANDLE;
		VkDescriptorPool pool = VK_NULL_HANDLE;
		std::vector<VkDescriptorSet> sets;

		bool CreateLayouts(VkDevice& device);
		bool AllocateSets(VkDevice& device, uint32_t count);
		bool UpdateSceneUBODescriptors(VkDevice& device, VulkanUniformBuffer& ubo);
	};

	struct RenderResources {
		BufferManager buffers;
		TextureManager textures;
		DescriptorSystem descriptors;

		VulkanUniformBuffer sceneUniformBuffer;
		VulkanInstanceBuffer instanceBuffer;
		VulkanParticleStorage particleStorage;

		bool Initialize(VulkanCoreContext& core);
		void Cleanup(VkDevice& device);
	};

	//==================== PIPELINE SYSTEM ===================//
	struct PipelineConfig {
		std::string vertexShaderName;
		std::string fragmentShaderName;
		std::string computeShaderName;
		RasterState rasterState;
	};

	struct RasterPipeline {
		VkPipeline pipeline = VK_NULL_HANDLE;
		VkPipelineLayout layout = VK_NULL_HANDLE;
		VkShaderModule vertShader = VK_NULL_HANDLE;
		VkShaderModule fragShader = VK_NULL_HANDLE;
	};

	struct ComputePipeline {
		VkPipeline pipeline = VK_NULL_HANDLE;
		VkPipelineLayout layout = VK_NULL_HANDLE;
		VkShaderModule computeShader = VK_NULL_HANDLE;
		VkDescriptorSetLayout descriptorLayout = VK_NULL_HANDLE;
		VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
	};

	struct PipelineSystem {
		RasterPipeline rasterPipeline;
		ComputePipeline computePipeline;

		bool CreateRaster(const PipelineConfig& config, VulkanCoreContext& core, const DescriptorSystem& desc);
		bool CreateCompute(const PipelineConfig& config, VulkanCoreContext& core, const DescriptorSystem& desc);
		bool ReloadShaders(const std::string& pipelineName);
		VkShaderModule LoadShaderToRasterPipeline(const char* path, shaderc_shader_kind kind, VulkanCoreContext& core, PipelineSystem& pipeline);
		VkShaderModule LoadShaderToComputePipeline(const char* path, shaderc_shader_kind kind, VulkanCoreContext& core, PipelineSystem& pipeline);
		PipelineConfig LoadPipelineConfig(const std::string& jsonFile);
	};

	//=================== PARTICLE SYSTEM ====================//
	struct VulkanParticleBuffer {
		VkBuffer buffer = VK_NULL_HANDLE;
		VkDeviceMemory memory = VK_NULL_HANDLE;
		uint32_t maxParticles = 10;
	};

	struct VulkanComputePipeline {
		VkPipeline pipeline = VK_NULL_HANDLE;
		VkPipelineLayout layout = VK_NULL_HANDLE;
		VkShaderModule computeShader = VK_NULL_HANDLE;
		VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
		VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
	};

	struct Particle {
		GW::MATH::GVECTORF position;
		GW::MATH::GVECTORF velocity;
		float lifetime = 5.0f;
		float age = 0.0f;
	};

	struct ParticleModule {
		VulkanComputePipeline compute;
		VulkanParticleBuffer buffer;
		VulkanParticleStorage storage;
		uint32_t maxParticles = 0;

		bool InitParticles(const VkPhysicalDevice& physicalDevice, const VkDevice& device);
		bool DispatchParticleCompute(VkCommandBuffer& cmd);
		bool ResetParticles();
	};

	//==================== CAMERA & SCENE ====================//
	struct CameraComponent
	{
		Camera camera;
		bool enableBillboarding = true;
	};

	struct SceneData
	{
		GW::MATH::GVECTORF sunDirection, sunColor, sunAmbient, camPos;
		GW::MATH::GMATRIXF viewMatrix, projectionMatrix;
		float deltaTime = 0.0f;
		float totalTime = 0.0f;
		uint32_t frameIndex = 0;
		GW::MATH::GMATRIXF viewProjection;
	};

	struct PerFrameResources {
		VkCommandBuffer cmd = VK_NULL_HANDLE;
		FrameSync sync;
		VkDescriptorSet descriptors = VK_NULL_HANDLE;
	};

	struct RenderFrameState {
		SceneData sceneData;
		CameraComponent camera;
		std::vector<PerFrameResources> perFrame;

		bool UpdateCamera();
		bool BeginFrame();
		bool EndFrame();
	};

	//================== MODEL / LEVEL SYSTEM =================//
	struct GeometryData {
		unsigned int indexStart, indexCount, vertexStart;
		inline bool operator<(const GeometryData a) const {
			return indexStart < a.indexStart;
		}
	};

	struct GPUInstance {
		GW::MATH::GMATRIXF transform;
		H2B::ATTRIBUTES matData;
	};

	struct MeshCollection {
		std::vector<entt::entity> entitties;
		GW::MATH::GOBBF collider;
	};

	struct ModelManager {
		std::map<std::string, MeshCollection> models;
	};

	struct CPULevel {
		std::string levelPath;
		std::string modelFolder;
		Level_Data levelData;
	};

	struct GPULevel {};

	struct LevelSystem {
		CPULevel cpuLevel;
		GPULevel gpuLevel;
	};

	//==================== RENDERER ROOT =====================//
	struct ImGuiResources {
		bool initialized = false;
		VkDescriptorPool pool = VK_NULL_HANDLE;
	};

	struct RendererConfig
	{
		std::string vertexShaderName = "../../../../Shaders/ModelBased/LevelVertex.hlsl";
		std::string fragmentShaderName = "../../../../Shaders/ModelBased/LevelPixel.hlsl";
		std::string computeShaderName;
		VkClearColorValue clearColor = { 0.2f, 0.2f, 0.2f, 1.0f };
		VkClearDepthStencilValue depthStencil = {0.75f, 0};
		float fovDegrees = 60.0f;
		float nearPlane = 0.1f;
		float farPlane = 1000.0f;
		VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
		bool enableAnisotropy = true;
		bool useComputeParticles = true;

		bool LoadFromJSON(const std::string& filepath);
		bool ApplyToPipelineConfig(struct PipelineConfig& outConfig);
	};

	struct VulkanRenderer
	{
		GW::GRAPHICS::GVulkanSurface vlkSurface;

		RendererConfig config;
		VulkanCoreContext core;

		RenderResources resources;
		PipelineSystem pipelines;

		ParticleModule particles;
		ImGuiResources imgui;

		RenderFrameState scene;

		uint32_t currentFrame = 0;
		float startTime = 0.0f;
	};

} // namespace DRAW

#endif // !DRAW_COMPONENTS_H

#ifndef DRAW_COMPONENTS_H
#define DRAW_COMPONENTS_H
#include "../../Camera/Camera.h"
#include "../../Utils/IuvoUtils.h"


namespace DRAW
{
	//*** TAGS ***//
	struct DoNotRenderTag {

	};

	struct ParticleRenderTag {};



	//*** COMPONENTS ***//
	struct VulkanRendererInitialization
	{
		std::string vertexShaderName;
		std::string fragmentShaderName;
		VkClearColorValue clearColor;
		VkClearDepthStencilValue depthStencil;
		float fovDegrees;
		float nearPlane;
		float farPlane;
		VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
		std::string computeShaderName;
		bool enableAnisotropy = true;
		bool useComputeParticles = true;
	};



	struct VulkanRenderer
	{
		GW::GRAPHICS::GVulkanSurface vlkSurface;
		VkDevice device = nullptr;
		VkPhysicalDevice physicalDevice = nullptr;
		VkRenderPass renderPass;
		VkShaderModule vertexShader = nullptr;
		VkShaderModule fragmentShader = nullptr;
		VkPipeline pipeline = nullptr;
		VkPipelineLayout pipelineLayout = nullptr;
		GW::MATH::GMATRIXF projMatrix;
		VkDescriptorSetLayout descriptorLayout = nullptr;
		VkDescriptorPool descriptorPool = nullptr;
		std::vector<VkDescriptorSet> descriptorSets;
		VkClearValue clrAndDepth[2];

		std::vector<VkFramebuffer> framebuffers;
		VkSwapchainKHR swapchain = VK_NULL_HANDLE;
		std::vector<VkImage> swapchainImages;
		std::vector<VkImageView> swapchainImageViews;

		VkCommandPool commandPool = VK_NULL_HANDLE;
		std::vector<VkCommandBuffer> commandBuffers;

		VkSemaphore imageAvailableSemaphore;
		VkSemaphore renderFinishedSemaphore;
		VkFence inFlightFence;
		bool imguiInitialized = false;
		float startTime = 0.0f;
		int currentFrameIndex = 0;
		VkDescriptorPool imguiDescriptorPool = VK_NULL_HANDLE;
	};



	struct VulkanDescriptor
	{
		VkDescriptorSetLayout layout;
		VkDescriptorPool pool;
		std::vector<VkDescriptorSet> sets;
	};

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

	struct VulkanParticleStorage {
		VkBuffer buffer;
		VkDeviceMemory memory;
		uint32_t count;
	};



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
		
	struct GeometryData
	{
		unsigned int indexStart, indexCount, vertexStart;
		inline bool operator < (const GeometryData a) const {
			return indexStart < a.indexStart;
		}
	};
	
	struct GPUInstance
	{
		GW::MATH::GMATRIXF	transform;
		H2B::ATTRIBUTES		matData;
	};

	struct VulkanGPUInstanceBuffer
	{
		unsigned long long element_count = 1;
		std::vector<VkBuffer> buffer;
		std::vector<VkDeviceMemory> memory;
	};

	struct SceneData
	{
		GW::MATH::GVECTORF sunDirection, sunColor, sunAmbient, camPos;
		GW::MATH::GMATRIXF viewMatrix, projectionMatrix;
		float deltaTime = 0.0f;
		float totalTime = 0.0f;
		uint32_t frameIndex;
		GW::MATH::GMATRIXF viewProjection;
	};

	struct VulkanUniformBuffer
	{
		std::vector<VkBuffer> buffer;
		std::vector<VkDeviceMemory> memory;
		uint32_t alignedSize;
		uint32_t bufferCount;
	};

	struct VulkanTexture {
		VkImage image;
		VkDeviceMemory memory;
		VkImageView view;
		VkSampler sampler;
	};


	struct CameraComponent
	{
		Camera camera;
		//GW::MATH::GMATRIXF camMatrix;
		bool enableBillboarding = true;

	};	

	struct CPULevel {
		std::string levelPath;
		std::string modelFolder;
		Level_Data levelData;
	};

	struct GPULevel {

	};

	struct MeshCollection {
		std::vector<entt::entity> entitties;
		GW::MATH::GOBBF collider;
	};

	struct ModelManager {
		std::map<std::string, MeshCollection> models;
	};

} // namespace DRAW
#endif // !DRAW_COMPONENTS_H

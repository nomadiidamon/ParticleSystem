#pragma once
#include "DrawComponents.h"

namespace DRAW
{


	bool VulkanCoreContext::InitDevice(VkInstance instance)
	{
		if (instance == VK_NULL_HANDLE) {
			std::cout << "InitDevice: invalid VkInstance\n";
			return false;
		}

		// Enumerate physical devices
		uint32_t physicalDeviceCount = 0;
		if (vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr) != VK_SUCCESS || physicalDeviceCount == 0) {
			std::cout << "InitDevice: No Vulkan physical devices found\n";
			return false;
		}

		std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
		if (vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data()) != VK_SUCCESS) {
			std::cout << "InitDevice: Failed to enumerate physical devices\n";
			return false;
		}

		// Choose a physical device with a graphics queue
		VkPhysicalDevice chosen = VK_NULL_HANDLE;
		uint32_t chosenQueueFamily = UINT32_MAX;

		for (auto& pd : physicalDevices) {
			uint32_t qCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(pd, &qCount, nullptr);
			std::vector<VkQueueFamilyProperties> qProps(qCount);
			vkGetPhysicalDeviceQueueFamilyProperties(pd, &qCount, qProps.data());

			for (uint32_t i = 0; i < qCount; ++i) {
				if (qProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
					chosen = pd;
					chosenQueueFamily = i;
					break;
				}
			}

			if (chosen != VK_NULL_HANDLE) break;
		}

		if (chosen == VK_NULL_HANDLE) {
			std::cout << "InitDevice: No physical device with graphics queue found\n";
			return false;
		}

		device.physicalDevice = chosen;
		device.graphicsQueueFamily = chosenQueueFamily;

		// Create logical device and graphics queue
		float queuePriority = 1.0f;
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = device.graphicsQueueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;

		VkPhysicalDeviceFeatures deviceFeatures{};
		// Enable common useful features (can be adjusted later)
		deviceFeatures.samplerAnisotropy = VK_TRUE;

		VkDeviceCreateInfo deviceCreateInfo{};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.queueCreateInfoCount = 1;
		deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
		deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
		deviceCreateInfo.enabledExtensionCount = 0;
		deviceCreateInfo.ppEnabledExtensionNames = nullptr;

		VkResult res = vkCreateDevice(device.physicalDevice, &deviceCreateInfo, nullptr, &device.device);
		if (res != VK_SUCCESS) {
			std::cout << "InitDevice: vkCreateDevice failed: " << res << "\n";
			return false;
		}

		vkGetDeviceQueue(device.device, device.graphicsQueueFamily, 0, &device.graphicsQueue);

		// Create a command pool for allocating command buffers from the graphics queue family
		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = device.graphicsQueueFamily;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		res = vkCreateCommandPool(device.device, &poolInfo, nullptr, &device.commandPool);
		if (res != VK_SUCCESS) {
			std::cout << "InitDevice: vkCreateCommandPool failed: " << res << "\n";
			vkDestroyDevice(device.device, nullptr);
			device.device = VK_NULL_HANDLE;
			return false;
		}

		return true;
	}

	bool VulkanCoreContext::InitSwapchain()
	{
		if (device.device == VK_NULL_HANDLE) {
			std::cout << "InitSwapchain: device not initialized\n";
			return false;
		}

		// Set sensible defaults if not set already
		if (swapchain.minImageCount < 2) swapchain.minImageCount = 2;
		if (swapchain.imageFormat == VK_FORMAT_UNDEFINED) swapchain.imageFormat = VK_FORMAT_B8G8R8A8_SRGB;
		if (swapchain.extent.width == 0 || swapchain.extent.height == 0) {
			swapchain.extent.width = 800;
			swapchain.extent.height = 600;
		}

		return true;
	}

	bool VulkanCoreContext::InitRenderPass()
	{
		if (device.device == VK_NULL_HANDLE) {
			std::cout << "InitRenderPass: device not initialized\n";
			return false;
		}

		// Choose color attachment format from swapchain (fallback if undefined)
		VkFormat colorFormat = (swapchain.imageFormat == VK_FORMAT_UNDEFINED) ? VK_FORMAT_B8G8R8A8_SRGB : swapchain.imageFormat;

		// Color attachment
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = colorFormat;
		colorAttachment.samples = renderPass.msaaSamples;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		// Depth attachment (choose a common depth format)
		VkAttachmentDescription depthAttachment{};
		depthAttachment.format = VK_FORMAT_D32_SFLOAT;
		depthAttachment.samples = renderPass.msaaSamples;
		depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		// Attachment references
		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentReference depthAttachmentRef{};
		depthAttachmentRef.attachment = 1;
		depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		// Subpass
		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;
		subpass.pDepthStencilAttachment = &depthAttachmentRef;

		// Subpass dependency
		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		VkResult res = vkCreateRenderPass(device.device, &renderPassInfo, nullptr, &renderPass.renderPass);
		if (res != VK_SUCCESS) {
			std::cout << "InitRenderPass: vkCreateRenderPass failed: " << res << "\n";
			return false;
		}

		return true;
	}

	bool VulkanCoreContext::InitCommandBuffers()
	{
		if (device.device == VK_NULL_HANDLE || device.commandPool == VK_NULL_HANDLE) {
			std::cout << "InitCommandBuffers: device or command pool not initialized\n";
			return false;
		}

		uint32_t count = std::max<uint32_t>(1, swapchain.actualImageCount == 0 ? 1 : static_cast<int>(swapchain.actualImageCount));
		commandBuffers.resize(count);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = device.commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		VkResult res = vkAllocateCommandBuffers(device.device, &allocInfo, commandBuffers.data());
		if (res != VK_SUCCESS) {
			std::cout << "InitCommandBuffers: vkAllocateCommandBuffers failed: " << res << "\n";
			commandBuffers.clear();
			return false;
		}

		return true;
	}

	VkCommandBuffer VulkanCoreContext::BeginSingleUseCmd()
	{
		if (device.device == VK_NULL_HANDLE || device.commandPool == VK_NULL_HANDLE || device.graphicsQueue == VK_NULL_HANDLE) {
			std::cout << "BeginSingleUseCmd: device/commandPool/graphicsQueue not ready\n";
			return VK_NULL_HANDLE;
		}

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = device.commandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer cmd = VK_NULL_HANDLE;
		if (vkAllocateCommandBuffers(device.device, &allocInfo, &cmd) != VK_SUCCESS) {
			std::cout << "BeginSingleUseCmd: vkAllocateCommandBuffers failed\n";
			return VK_NULL_HANDLE;
		}

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		if (vkBeginCommandBuffer(cmd, &beginInfo) != VK_SUCCESS) {
			std::cout << "BeginSingleUseCmd: vkBeginCommandBuffer failed\n";
			vkFreeCommandBuffers(device.device, device.commandPool, 1, &cmd);
			return VK_NULL_HANDLE;
		}

		return cmd;
	}

	void VulkanCoreContext::EndSingleUseCmd(VkCommandBuffer cmd)
	{
		if (cmd == VK_NULL_HANDLE || device.device == VK_NULL_HANDLE || device.graphicsQueue == VK_NULL_HANDLE) {
			std::cout << "EndSingleUseCmd: invalid parameters\n";
			return;
		}

		if (vkEndCommandBuffer(cmd) != VK_SUCCESS) {
			std::cout << "EndSingleUseCmd: vkEndCommandBuffer failed\n";
			vkFreeCommandBuffers(device.device, device.commandPool, 1, &cmd);
			return;
		}

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &cmd;

		if (vkQueueSubmit(device.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
			std::cout << "EndSingleUseCmd: vkQueueSubmit failed\n";
			vkFreeCommandBuffers(device.device, device.commandPool, 1, &cmd);
			return;
		}

		vkQueueWaitIdle(device.graphicsQueue);

		vkFreeCommandBuffers(device.device, device.commandPool, 1, &cmd);
	}

}
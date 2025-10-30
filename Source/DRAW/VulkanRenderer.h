#pragma once
#include "DrawComponents.h"

namespace DRAW
{
	VkViewport CreateViewportFromWindowDimensions(unsigned int windowWidth, unsigned int windowHeight);

	VkRect2D CreateScissorFromWindowDimensions(unsigned int windowWidth, unsigned int windowHeight);

	void InitializeDescriptors(entt::registry& registry, entt::entity entity);

	void InitializeGraphicsPipeline(entt::registry& registry, entt::entity entity);

	//*** SYSTEMS ***//

	// run this code when a VulkanRenderer component is connected
	void Construct_VulkanRenderer(entt::registry& registry, entt::entity entity);

	// run this code when a VulkanRenderer component is updated
	void Update_VulkanRenderer(entt::registry& registry, entt::entity entity);

	// run this code when a VulkanRenderer component is updated
	void Destroy_VulkanRenderer(entt::registry& registry, entt::entity entity);

	// Use this MACRO to connect the EnTT Component Logic
	//CONNECT_COMPONENT_LOGIC();

} // namespace DRAW
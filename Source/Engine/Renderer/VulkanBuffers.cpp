#include "DrawComponents.h"
#include "../Renderer/Renderer.h"
namespace DRAW
{
	//*** HELPERS ***//
	void AddSceneData(entt::registry& registry, entt::entity entity)
	{
		auto& sceneData = registry.emplace<SceneData>(entity,
			SceneData{ { -1.0f, -1.0f, 2.0f }, { 0.9f, 0.9f, 0.9f }, { 0.2f, 0.2f, 0.2f } });

		GW::MATH::GVector::NormalizeF(sceneData.sunDirection, sceneData.sunDirection);
		auto& renderer = registry.get<Engine::Renderer>(entity);
		sceneData.projectionMatrix = renderer.GetVulkanRenderer().scene.camera.camera.camMatrix;
	}

	//*** SYSTEMS ***//
	// Forward Declare
	void Destroy_VulkanVertexBuffer(entt::registry& registry, entt::entity entity);

	void Update_VulkanVertexBuffer(entt::registry& registry, entt::entity entity) {

	}

	void Destroy_VulkanVertexBuffer(entt::registry& registry, entt::entity entity) {

	}

	// Forward declare
	void Destroy_VulkanIndexBuffer(entt::registry& registry, entt::entity entity);

	void Update_VulkanIndexBuffer(entt::registry& registry, entt::entity entity) {
	}

	void Destroy_VulkanIndexBuffer(entt::registry& registry, entt::entity entity) {

	}

	void Construct_VulkanGPUInstanceBuffer(entt::registry& registry, entt::entity entity) {

	}
		
	// Forward declare
	void Destroy_VulkanGPUInstanceBuffer(entt::registry& registry, entt::entity entity);

	void Update_VulkanGPUInstanceBuffer(entt::registry& registry, entt::entity entity) {

	}

	void Destroy_VulkanGPUInstanceBuffer(entt::registry& registry, entt::entity entity) {

	}

	void Construct_VulkanUniformBuffer(entt::registry& registry, entt::entity entity) {


	}

	void Update_VulkanUniformBuffer(entt::registry& registry, entt::entity entity) {

	}


	void Destroy_VulkanUniformBuffer(entt::registry& registry, entt::entity entity) {

	}

	// Use this MACRO to connect the EnTT Component Logic
	CONNECT_COMPONENT_LOGIC() {
		// register the Window component's logic
		registry.on_update<VulkanVertexBuffer>().connect<Update_VulkanVertexBuffer>();
		registry.on_destroy<VulkanVertexBuffer>().connect<Destroy_VulkanVertexBuffer>();

		registry.on_update<VulkanIndexBuffer>().connect<Update_VulkanIndexBuffer>();
		registry.on_destroy<VulkanIndexBuffer>().connect<Destroy_VulkanIndexBuffer>();

		registry.on_construct<VulkanInstanceBuffer>().connect<Construct_VulkanGPUInstanceBuffer>();
		registry.on_update<VulkanInstanceBuffer>().connect<Update_VulkanGPUInstanceBuffer>();
		registry.on_destroy<VulkanInstanceBuffer>().connect<Destroy_VulkanGPUInstanceBuffer>();

		registry.on_construct<VulkanUniformBuffer>().connect<Construct_VulkanUniformBuffer>();
		registry.on_update<VulkanUniformBuffer>().connect<Update_VulkanUniformBuffer>();
		registry.on_destroy<VulkanUniformBuffer>().connect<Destroy_VulkanUniformBuffer>();
	}

} // namespace DRAW
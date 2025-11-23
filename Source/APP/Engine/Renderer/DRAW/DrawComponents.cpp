#include "DrawComponents.h"
#include "../../Particle/ParticleComponents.h"
#include "../../Particle/ParticleSystemComponents.h"

namespace DRAW
{

	VkInstance& VulkanRenderer::GetInstance()
	{
		VkInstance* instance = nullptr;
		vlkSurface.GetInstance((void**)instance);
		return *instance;
	}

	uint32_t& VulkanRenderer::GetGraphicsQueueFamilyIndex()
	{
		uint32_t* queueFamilyIndexPtr = nullptr;
		uint32_t _ = 0;
		vlkSurface.GetQueueFamilyIndices(*queueFamilyIndexPtr, _);
		return *queueFamilyIndexPtr;

	}

	VkQueue& VulkanRenderer::GetGraphicsQueue()
	{
		VkQueue* graphicsQueuePtr = nullptr;
		vlkSurface.GetGraphicsQueue((void**)graphicsQueuePtr);
		return *graphicsQueuePtr;
	}


	VkDescriptorPool& VulkanRenderer::GetImGuiDescriptorPool()
	{
		return imguiPool;
	}

	uint32_t& VulkanRenderer::GetMinImageCount()
	{
		return minImageCount;
	}

	uint32_t& VulkanRenderer::GetSwapchainImageCount()
	{
		return swapchainImageCount;
	}

	VkSampleCountFlags VulkanRenderer::GetMSAASamples()
	{
		return VK_SAMPLE_COUNT_1_BIT;
	}

	void VulkanRenderer::CheckVk(VkResult result)
	{
	}

	void Construct_CPULevel(entt::registry& registry, entt::entity entity) {

		auto& cpuLevel = registry.get<CPULevel>(entity);

		// Load the level data
		GW::SYSTEM::GLog log;
		log.Create("level.log");
		log.EnableConsoleLogging(true);

		if (cpuLevel.levelData.LoadLevel(cpuLevel.levelPath.c_str(), cpuLevel.modelFolder.c_str(), log)) {
			log.Log("[CUSTOM LOG MESSAGE]\t Sucessfully Loaded the Level.");
		}
		else {
			log.Log("[CUSTOM LOG MESSAGE]\t Failed to Load Level.");
		}
	}

	void Construct_GPULevel(entt::registry& registry, entt::entity entity) {

		auto cpuLevel = registry.try_get<CPULevel>(entity);
		if (cpuLevel != nullptr) {

			registry.emplace<DRAW::VulkanVertexBuffer>(entity);
			registry.emplace<std::vector<H2B::VERTEX>>(entity, std::vector<H2B::VERTEX>{cpuLevel->levelData.levelVertices});
			registry.patch<DRAW::VulkanVertexBuffer>(entity);

			registry.emplace<DRAW::VulkanIndexBuffer>(entity);
			registry.emplace<std::vector<unsigned int>>(entity, std::vector<unsigned int>{cpuLevel->levelData.levelIndices});
			registry.patch<DRAW::VulkanIndexBuffer>(entity);

			auto& list = registry.ctx().emplace<DRAW::ModelManager>();

			for (const auto& obj : cpuLevel->levelData.blenderObjects) {
				const auto& model = cpuLevel->levelData.levelModels[obj.modelIndex];

				MeshCollection collect;
				collect.collider = cpuLevel->levelData.levelColliders[obj.modelIndex];
				// Loop through every mesh in the model using meshStart and meshCount
				for (unsigned int i = 0; i < model.meshCount; ++i) {
					const auto& mesh = cpuLevel->levelData.levelMeshes[model.meshStart + i];

					// Create a new entity for each mesh
					auto ent = registry.create();
					registry.emplace<UNIVERSAL::NameComponent>(ent, UNIVERSAL::NameComponent{ obj.blendername });
					registry.emplace<DRAW::GeometryData>(ent, DRAW::GeometryData{ model.indexStart, model.indexCount, model.vertexStart });

					registry.emplace<DRAW::GPUInstance>(ent,
						DRAW::GPUInstance{
							cpuLevel->levelData.levelTransforms[obj.transformIndex],
							cpuLevel->levelData.levelMaterials[model.materialStart + mesh.materialIndex].attrib
						}
					);

					if (model.isDynamic) {
						registry.emplace<DRAW::DoNotRenderTag>(ent);
						collect.entitties.push_back(ent);
					}
				}
				if (model.isCollidable) {
					auto collidableEntity = registry.create();
					registry.emplace<UNIVERSAL::CollisionTag>(collidableEntity); // Add Collidable tag
					registry.emplace<MeshCollection>(collidableEntity, collect); // Add MeshCollection
					registry.emplace<UNIVERSAL::TransformComponent>(collidableEntity,
						PARTICLE::TransformComponent{ cpuLevel->levelData.levelTransforms[obj.transformIndex] }); // Add Transform

					registry.emplace<UNIVERSAL::NameComponent>(collidableEntity, UNIVERSAL::NameComponent{ obj.blendername }); // Add Name tag

					if (model.isCollidable && !model.isDynamic) {
						registry.emplace<UNIVERSAL::ObstacleTag>(collidableEntity);
					}

				}
				if (model.isDynamic) {
					list.models[model.filename] = collect;
				}

			}
		}
		else {
			GW::SYSTEM::GLog log;
			log.Create("level.log");
			log.Log("[CUSTOM LOG MESSAGE]\t Failed to find CPULevel component.");
		}
	}


	void Destroy_MeshCollection(entt::registry& registry, entt::entity entity) {
		// Check if the entity has a MeshCollection
		if (auto* meshCollection = registry.try_get<MeshCollection>(entity)) {
			// Iterate through all entities in the MeshCollection
			for (auto meshEntity : meshCollection->entitties) {
				// Destroy each mesh entity
				if (registry.valid(meshEntity)) {
					registry.destroy(meshEntity);
				}
			}
		}
	}

	CONNECT_COMPONENT_LOGIC() {
		// register the Window component's logic
		registry.on_construct<CPULevel>().connect<Construct_CPULevel>();
		registry.on_construct<GPULevel>().connect<Construct_GPULevel>();
		registry.on_destroy<MeshCollection>().connect<Destroy_MeshCollection>();
	}

}
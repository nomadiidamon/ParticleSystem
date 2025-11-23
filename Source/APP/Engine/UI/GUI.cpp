#pragma once 
#include "../../../precompiled.h"
#include "../Renderer/DRAW/DrawComponents.h"
#include "../../Window.hpp"
#include "../Renderer/Renderer.h"
#include "GUI.h"

namespace Engine {
	namespace UI {

		GUIManager::GUIManager(entt::registry& registry) : m_registry(registry) {

		}

		GUIManager::~GUIManager() {

		}

		void GUIManager::Update(float deltaTime) {
			/// TODO: add callbacks to system events if needed
		}

		void GUIManager::RenderFrame(float deltaTime)
		{
			BeginFrame();
			RenderGUI();   // Loop all UILayers
			EndFrame();
		}

		void GUIManager::Init() {
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();

			// Enable features
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

			ImGui::StyleColorsDark();

			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				ImGuiStyle& style = ImGui::GetStyle();
				style.WindowRounding = 0.0f;
				style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			}

			auto appEntitty = m_registry.view<APP::Window>().front();
			auto& windowComp = m_registry.get<APP::Window>(appEntitty);
			auto& gwWindow = m_registry.get<GW::SYSTEM::GWindow>(appEntitty);
			GW::SYSTEM::UNIVERSAL_WINDOW_HANDLE gwHandle;
			Renderer& renderer = m_registry.get<Renderer>(m_registry.view<Renderer>().front());
			// ---------------------------
			// BACKEND: WIN32
			// ---------------------------
			gwWindow.GetWindowHandle(gwHandle);
			ImGui_ImplWin32_Init(gwHandle.window);

			// ---------------------------
			// BACKEND: VULKAN
			// ---------------------------

			ImGui_ImplVulkan_InitInfo initInfo = {};
			initInfo.Instance = renderer.GetInstance();
			initInfo.PhysicalDevice = renderer.GetPhysicalDevice();
			initInfo.Device = renderer.GetDevice();
			initInfo.QueueFamily = renderer.GetGraphicsQueueFamilyIndex();
			initInfo.Queue = renderer.GetGraphicsQueue();

			initInfo.PipelineCache = VK_NULL_HANDLE;
			initInfo.DescriptorPool = renderer.GetImGuiDescriptorPool();

			initInfo.MinImageCount = renderer.GetMinImageCount();
			initInfo.ImageCount = renderer.GetSwapchainImageCount();

			initInfo.PipelineInfoMain.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
			initInfo.CheckVkResultFn = renderer.CheckVk;

			ImGui_ImplVulkan_Init(&initInfo);

			printf("GUIManager initialized (Vulkan + Win32)\n");
		}

		void GUIManager::Shutdown() {
			/// TODO: Shutdown renderer backends

			ImGui::DestroyContext();
		}

		void GUIManager::BeginFrame() {
			/// Start new frame for renderer backends

			ImGui::NewFrame();
		}

		void GUIManager::RenderGUI()
		{
			/// TODO: loop through ui layers and render them

			ImGui::Begin("Demo Window");
			ImGui::Text("Hello from GUIManager + ImGui!");
			ImGui::End();

			/// TODO: loop through UIRenderable components and render them

		}

		void GUIManager::EndFrame()
		{
			ImGuiIO& io = ImGui::GetIO();

			// Render ImGui
			ImGui::Render();

			/// TODO: Submit draw data to renderer
		}

		void GUIManager::RenderLayers(float deltaTime)
		{
		}

		void GUIManager::UpdateLayers(float deltaTime)
		{
		}

		void OnConstruct_GUIManager(entt::registry& registry, entt::entity entity)
		{
			auto& guiManagerComp = registry.get<GUIManagerComponent>(entity);
			guiManagerComp.manager.Init();
		}

		void OnUpdate_GUIManager(entt::registry& registry, entt::entity entity)
		{
			auto& guiManagerComp = registry.get<GUIManagerComponent>(entity);
			guiManagerComp.manager.Update(0.f);
		}

		void OnDestroy_GUIManager(entt::registry& registry, entt::entity entity)
		{
			auto& guiManagerComp = registry.get<GUIManagerComponent>(entity);
			guiManagerComp.manager.Shutdown();
		}


		CONNECT_COMPONENT_LOGIC() {
			registry.on_construct<GUIManager>().connect<OnConstruct_GUIManager>();
			registry.on_update<GUIManager>().connect<OnUpdate_GUIManager>();
			registry.on_destroy<GUIManager>().connect<OnDestroy_GUIManager>();
		}


	} // namespace GUI
}
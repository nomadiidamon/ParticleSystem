#include <iostream>
#include <windows.h>
#include "APP/App.h"
#include "precompiled.h"


// Forward declare Win32 proc
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int main()
{
    // 1. Create Win32 window
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L,
                      GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
                      L"ImGui Vulkan Example", NULL };
    RegisterClassEx(&wc);
    HWND hwnd = CreateWindow(wc.lpszClassName, L"Dear ImGui Vulkan Example",
        WS_OVERLAPPEDWINDOW, 100, 100, 1280, 720,
        NULL, NULL, wc.hInstance, NULL);

    // 2. Initialize Vulkan (instance, device, swapchain, etc.)
    // Normally you'd create VkInstance, VkDevice, VkRenderPass, etc.
    // For simplicity, refer to ImGui’s example_vulkan/main.cpp for full Vulkan setup.
    // Here we’ll assume you have a VkInstance, VkDevice, VkRenderPass ready.
    //VkInstance instance = ...;
    //VkDevice device = ...;
    //VkRenderPass render_pass = ...;
    //VkQueue queue = ...;
    //uint32_t queue_family = ...;
    //VkDescriptorPool descriptor_pool = ...;

    // 3. Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    // 4. Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplVulkan_InitInfo init_info = {};
    //init_info.Instance = instance;
    //init_info.PhysicalDevice = /* your physical device */;
    //init_info.Device = device;
    //init_info.QueueFamily = queue_family;
    //init_info.Queue = queue;
    init_info.PipelineCache = VK_NULL_HANDLE;
    //init_info.DescriptorPool = descriptor_pool;
    init_info.MinImageCount = 2;
    init_info.ImageCount = 2;
    //init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
	init_info.PipelineInfoMain.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    init_info.CheckVkResultFn = nullptr;
    ImGui_ImplVulkan_Init(&init_info);

    //// Upload Fonts
    //{
    //    VkCommandBuffer command_buffer = /* begin single-use command buffer */;
    //    ImGui_ImplVulkan_CreateFontsTexture(command_buffer);
    //    /* end command buffer and submit */;
    //    ImGui_ImplVulkan_DestroyFontUploadObjects();
    //}

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    // 5. Main loop
    bool done = false;
    while (!done)
    {
        MSG msg;
        while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Start new frame
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // Draw GUI
        ImGui::Begin("Hello!");
        ImGui::Text("This is Dear ImGui running on Vulkan!");
        ImGui::End();

        // Render
        ImGui::Render();
        // Record Vulkan commands to render ImGui draw data
        //ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), /*your_command_buffer*/);
    }

    // Cleanup
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    DestroyWindow(hwnd);
    UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Win32 WndProc
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;
    switch (msg)
    {
    case WM_SIZE:
        if (wParam != SIZE_MINIMIZED)
        {
            // handle swapchain resize here
        }
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

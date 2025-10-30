#include <iostream>
#include "APP/App.h"
#include "precompiled.h"





int main()
{
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

        // Start the frame
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // Your ImGui content
        ImGui::Begin("Hello Vulkan!");
        ImGui::Text("This is Dear ImGui running on Vulkan + Win32!");
        ImGui::End();

        // Render
        ImGui::Render();
        // Normally you'd record draw commands here and present to a swapchain
        // (omitted for brevity)
    }

    return 0;
}



#include <iostream>
#include <windows.h>
#include "precompiled.h"


int main()
{
	std::cout << "Hello, World!\n" << std::endl;

	// initialize ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

	// start a frame
	ImGui::NewFrame();

	// create a window
	ImGui::Begin("Hello!");

	ImGui::Text("This is some useful text.");
	ImGui::End();

	ImGui::Render();

	// Cleanup
	ImGui::DestroyContext();

	return 0;
}
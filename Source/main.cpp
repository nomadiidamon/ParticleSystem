#include <iostream>
#include <windows.h>
#include "precompiled.h"


int main()
{
	std::cout << "Hello, World!\n" << std::endl;

	// initialize ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

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
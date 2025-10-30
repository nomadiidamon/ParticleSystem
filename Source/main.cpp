#include <iostream>
#include "App/App.h"



int main()
{
	std::cout << "Hello, World!\n" << std::endl;


	// Create application window

	ImGui::Begin("Hello!");
	ImGui::ShowAboutWindow();
	ImGui::End();
	//App app;
	return -1; // failed to create Gateware application
}
#include <iostream>
#include "App/App.h"



int main()
{
	std::cout << "Hello, World!\n" << std::endl;


	// Create application window
	while (true)
	{
		ImGui::ShowAboutWindow();
		ImGui::NewFrame();
	}
	//App app;
	return -1; // failed to create Gateware application
}
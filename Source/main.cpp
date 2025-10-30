#include <iostream>
#include "App/App.h"

int main()
{
	std::cout << "Hello, World!\n" << std::endl;

	ImGui::Begin("Hello, world!");

	bool active = true;
	while (active)
	{
		// run the app
		ImGui::NewFrame();
	}

	//App app;
	return -1; // failed to create Gateware application
}
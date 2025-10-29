#include <iostream>
#include "App/App.h"

int main()
{
	std::cout << "Hello, World!\n" << std::endl;

	ImGui::Begin("Hello, world!");
	ImGui::End();

	//App app;
	return -1; // failed to create Gateware application
}
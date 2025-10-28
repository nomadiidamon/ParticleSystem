// Iuvare.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "Iuvo.h"

int main()
{
	Iuvo::MemoryNet();
	srand(NULL);

	IuvoConsole::Console console;


	console.Write("Salvete amici! \t", IuvoConsole::Yellow);
	console.WriteLine("Hello friends!", IuvoConsole::Cyan);
	console.PressAnyKeyToContinue();

	console.Write("Iuvo", IuvoConsole::Yellow); 
	console.Write(" comes from the Latin verb ", IuvoConsole::Cyan);
	console.Write("Iuvare ", IuvoConsole::Yellow);
	console.WriteLine("and means 'I help' or 'I am helping'.", IuvoConsole::Cyan);
	console.PressAnyKeyToContinue();

	console.WriteLine("The testing of Iuvo functions has begun...", IuvoConsole::Cyan);
	console.PressAnyKeyToContinue();



	console.WriteLine("The testing of Iuvo functions has finished. \n", IuvoConsole::Cyan);
	console.Write("Valete!", IuvoConsole::Yellow);
	console.WriteLine(" Goodbye!", IuvoConsole::Cyan);
}
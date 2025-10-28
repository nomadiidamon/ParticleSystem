#ifndef IUVO_CONSOLE
#define IUVO_CONSOLE

#include <iostream>
#include <string>
#include <Windows.h>
#include <conio.h>

/// <summary>
/// This namespace has limited functionality as it is still being developed
/// Some functions may not work as expected yet
/// </summary>
namespace IuvoConsole {

	enum ConsoleColor
	{
		Black,
		Red,
		Green,
		Yellow,
		Blue,
		Magenta,
		Cyan,
		White,
		Default = 9
	};

	static const std::string ESC = "\x1B";
	static int windowWidth;
	static int windowHeight;

	class Console
	{
		HANDLE consoleHandle;
		HWND hwnd;

	public:


		Console(int width = 500, int height = 300) {
			windowWidth = width;
			windowHeight = height;
			InitializeConsole();
		}

		void InitializeConsole() {
			consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
			hwnd = GetConsoleWindow();
			
		}

		/// <summary>
		/// 
		/// ***** UNDER DEVELOPMENT *****
		/// NOT FUNCTIONING PROPERLY *****
		/// 
		/// </summary>
		void ResizeWindow(int width, int height)
		{

			COORD bufferSize = { (SHORT)width, (SHORT)height };
			SetConsoleScreenBufferSize(consoleHandle, bufferSize);


			SMALL_RECT windowSize = { 0, 0, (SHORT)(width - 1), (SHORT)(height - 1) };
			SetConsoleWindowInfo(consoleHandle, TRUE, &windowSize);

			std::cout << "Console window resized to " << width << "x" << height << " (characters)" << std::endl;

			HWND hwnd = GetConsoleWindow();

		}

		/// <summary>
		/// 
		/// ***** UNDER DEVELOPMENT *****
		/// NOT FUNCTIONING PROPERLY *****
		/// 
		/// <summary>
		void SetConsoleFont(int fontWidth, int fontHeight, const std::wstring& fontName = L"Consolas") {
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

			CONSOLE_FONT_INFOEX fontInfo;
			fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
			fontInfo.nFont = 0;
			fontInfo.dwFontSize.X = fontWidth;   // Set font width in pixels
			fontInfo.dwFontSize.Y = fontHeight;  // Set font height in pixels
			fontInfo.FontFamily = FF_DONTCARE;
			fontInfo.FontWeight = FW_NORMAL;

			wcscpy_s(fontInfo.FaceName, fontName.c_str());

			SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);

			std::wcout << L"Console font set to: " << fontName << L" with size " << fontWidth << L"x" << fontHeight << L" pixels." << std::endl;
		}

		/// <summary>
		/// Adjusts the cursor position to the designated location
		/// </summary>
		/// <param name="x"> is the coordinate for the new horizontal position</param>
		/// <param name="y"> is the coordinate for the new verticle position</param>
		void SetCursorPosition(int x, int y)
		{
			std::cout << ESC << "[" << y << ";" << x << "H";
		}

		/// <summary>
		/// Adjusts the cursor position by x characters horizontally
		/// </summary>
		/// <param name="x"> is the number of characters away from the left border</param>
		void SetCursorLeft(int x)
		{
			std::cout << ESC << "[" << x << "G";
		}

		/// <summary>
		/// Changes the text color of output messages. Changes will persist until reset manually or application ends
		/// </summary>
		/// <param name="foreColor"> is the new text color</param>
		void SetForegroundColor(ConsoleColor foreColor)
		{
			std::cout << ESC << "[" << foreColor + 30 << "m";
		}

		/// <summary>
		/// Changes the text color of output messages. Changes will persist until reset manually or application ends
		/// </summary>
		/// <param name="r"> is the red color componenet</param>
		/// <param name="g"> is the green color component</param>
		/// <param name="b"> is the blue color component</param>
		void SetForegroundColor(int r, int g, int b)
		{
			std::cout << ESC << "[38;2;" << r << ";" << g << ";" << b << "m";
		}

		/// <summary>
		/// Changes the background color of output messages. Changes will persist until reset manually or application ends
		/// </summary>
		/// <param name="foreColor"> is the new text color</param>
		void SetBackgroundColor(ConsoleColor backColor)
		{
			std::cout << ESC << "[" << backColor + 40 << "m";
		}

		/// <summary>
		/// Changes the background color of output messages. Changes will persist unitl reset manually or application ends
		/// </summary>
		/// <param name="r"> is the red color componenet</param>
		/// <param name="g"> is the green color component</param>
		/// <param name="b"> is the blue color component</param>
		void SetBackgroundColor(int r, int g, int b)
		{
			std::cout << ESC << "[48;2;" << r << ";" << g << ";" << b << "m";
		}

		/// <summary>
		/// Completely resets the Console to its default state, including foreground and background colors
		/// </summary>
		void Reset()
		{
			std::cout << ESC << "[0m";
		}

		/// <summary>
		/// Clears the screen and moves the cursor to the top-left
		/// </summary>
		void Clear()
		{
			std::cout << ESC << "[2J" << ESC << "[H";
		}

		/// <summary>
		/// Writes a string of text to the console window WITHOUT a new line
		/// </summary>
		/// <param name="message"> to be written to the console window</param>
		void Write(std::string message)
		{
			std::cout << message;
		}

		/// <summary>
		/// Writes a string of text to the console window WITHOUT a new line. Includes options for adjusting the
		/// foreground/background colors. Colors are reset to defaults at the end of the function call.
		/// </summary>
		/// <param name="message"> to be written to the console</param>
		/// <param name="foreColor"> to become the new text color</param>
		/// <param name="backColor"> to become the new background color</param>
		void Write(std::string message, ConsoleColor foreColor, ConsoleColor backColor = ConsoleColor::Black)
		{
			SetForegroundColor(foreColor);
			SetBackgroundColor(backColor);
			std::cout << message;
			Reset();
		}

		/// <summary>
		/// Writes a string of text to the console window WITH a new line
		/// </summary>
		/// <param name="message"> to be written to the console window</param>
		void WriteLine(std::string message)
		{
			std::cout << message << std::endl;
		}

		/// <summary>
		/// Writes a string of text to the console window WITH a new line. Includes options for adjusting 
		/// the foreground and background colors. Colors are reset to defaults at the end of the function call.
		/// </summary>
		/// <param name="message"> to be written to the console</param>
		/// <param name="foreColor"> to become the new text color</param>
		/// <param name="backColor"> to become the new background color</param>
		void WriteLine(std::string message, ConsoleColor foreColor, ConsoleColor backColor = ConsoleColor::Black)
		{
			SetForegroundColor(foreColor);
			SetBackgroundColor(backColor);
			std::cout << message;
			Reset();
			std::cout << std::endl;
		}

		/// <summary>
		/// Outputs a number along with a given string. Used for making a numbered list WITHOUT a new line after the input listItem.
		/// </summary>
		/// <param name="index"> is the number of the item to be listed</param>
		/// <param name="listItem"> is the string that will be output</param>
		void WriteNumberedList(int index, std::string listItem) {
			std::cout << std::to_string(index) << ": " << listItem;
		}

		/// <summary>
		/// Outputs a number along with a given string. Used for making a numbered list WITHOUT a new line after the input listItem.
		/// Additionally allows color adjustments
		/// </summary>
		/// <param name="index"> is the number of the item to be listed</param>
		/// <param name="listItem"> is the string that will be output</param>
		void WriteNumberedList(int index, ConsoleColor indexForeColor, std::string listItem, ConsoleColor listItemForeColor, ConsoleColor backgroundColor = ConsoleColor::Black) {
			SetForegroundColor(indexForeColor);
			std::cout << std::to_string(index) << ": ";
			SetForegroundColor(listItemForeColor);
			std::cout << listItem;
		}

		/// <summary>
		/// Asks the user to press a key before advancing to the next screen/function call
		/// </summary>
		void PressAnyKeyToContinue()
		{
			std::cout << "\nPress any key to continue...\n";

			while (!_kbhit()) {} // wait for input

			_getch(); // clear buffer of input
			Clear();
		}
	};



}

#endif
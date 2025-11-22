#include "IuvoUtils.h"

namespace Iuvo
{

	std::string ReadFileIntoString(const char* filePath)
	{
		std::string output;
		unsigned int stringLength = 0;
		GW::SYSTEM::GFile file;

		file.Create();
		file.GetFileSize(filePath, stringLength);

		if (stringLength > 0 && +file.OpenBinaryRead(filePath))
		{
			output.resize(stringLength);
			file.Read(&output[0], stringLength);
		}
		else
			std::cout << "ERROR: File \"" << filePath << "\" Not Found!" << std::endl;

		return output;
	}


	// Detects memory leaks
	void MemoryNet()
	{

		// Change this number to the line number the Output window shows you
		// to follow a memory leak. Put -1 to disable.
#define MEMORY_LEAK_LINE -1


		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		_CrtSetBreakAlloc(MEMORY_LEAK_LINE); // DO NOT COMMENT OUT THIS LINE
		_CrtDumpMemoryLeaks();
	}
}
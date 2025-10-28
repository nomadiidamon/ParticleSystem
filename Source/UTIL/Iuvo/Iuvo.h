#ifndef IUVO
#define IUVO

#include "IuvoConsole.h"
#include "IuvoInput.h"
#include "IuvoNames.h"
#include "IuvoRaster.h"
#include "IuvoRPG.h"

namespace Iuvo
{
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

};

#endif
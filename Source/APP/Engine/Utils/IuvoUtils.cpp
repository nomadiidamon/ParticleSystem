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
	void MemoryNet()
	{
		// Change this number to the line number the Output window shows you
		// to follow a memory leak. Put -1 to disable.
#define MEMORY_LEAK_LINE -1

		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		_CrtSetBreakAlloc(MEMORY_LEAK_LINE); // DO NOT COMMENT OUT THIS LINE
		_CrtDumpMemoryLeaks();
	}

	static bool FloatEquals(float a, float b) {
		return fabsf(a - b) <= EPS;
	}
	inline float DegToRad(float d) { return d * (PI_F / 180.0f); }

	static std::mt19937& GetRNG() {
		static std::random_device rd;
		static std::mt19937 mt(rd());
		return mt;
	}
	static float RandFloat(float a, float b) {
		std::uniform_real_distribution<float> dist(a, b);
		return dist(GetRNG());
	}
	static int RandInt(int a, int b) {
		std::uniform_int_distribution<int> dist(a, b);
		return dist(GetRNG());
	}

	static bool RandBool(float trueProbability = 0.5f) {
		std::bernoulli_distribution dist(trueProbability);
		return dist(GetRNG());
	}

	static unsigned char RandChar() {
		std::uniform_int_distribution<int> dist(0, 255);
		return static_cast<unsigned char>(dist(GetRNG()));
	}

	static std::string GenerateHex(const unsigned int length) {
		const char hexChars[] = "0123456789ABCDEF";
		std::string hexString;
		hexString.reserve(length);
		for (unsigned int i = 0; i < length; ++i) {
			hexString += hexChars[RandInt(0, 15)];
		}
		return hexString;
	}

	static std::string GenerateUUID() {
		return GenerateHex(8)
			+ "-" + GenerateHex(4)
			+ "-" + GenerateHex(4)
			+ "-" + GenerateHex(4)
			+ "-" + GenerateHex(12);
	}
}
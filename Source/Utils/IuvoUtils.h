#ifndef IUVO_UTIL_H
#define IUVO_UTIL_H
#include <random>
#include "CCL.h"
#include "h2bParser.h"
#include "XTime.h"
#include "load_data_oriented.h"

static entt::registry registry;

namespace Iuvo {
	std::string ReadFileIntoString(const char* filePath);
	void MemoryNet();

	constexpr float EPS = 1e-6f;
	static constexpr float PI_F = 3.14159265358979323846f;
	static bool FloatEquals(float a, float b);
	inline float DegToRad(float d);

	static std::mt19937& GetRNG();
	static float RandFloat(float a, float b);
	static int RandInt(int a, int b);
	static bool RandBool(float trueProbability);
	static unsigned char RandChar();
	static std::string GenerateHex(const unsigned int length);
	static std::string GenerateUUID();

	/// TODO: Create random funcs for Gateware math types

} // namespace Iuvo

#endif // !IUVO_UTIL_H

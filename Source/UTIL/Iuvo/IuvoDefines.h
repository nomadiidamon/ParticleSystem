#ifndef IUVO_DEFINES
#define IUVO_DEFINES

#include <iostream>
#include <vector>
#include <algorithm>

namespace IuvoDefines {

	// Rasterization basics
	constexpr int RASTER_WIDTH = 1200;
	constexpr int RASTER_HEIGHT = 800;
	constexpr int RASTER_SIZE = RASTER_HEIGHT * RASTER_WIDTH;
	unsigned int* DESTINATION_ARRAY = new unsigned int[RASTER_SIZE];
	constexpr float CUBE_ROT_SPEED = 0.0025f;
	constexpr float ASPECT_RATIO = ((float)RASTER_HEIGHT / RASTER_WIDTH);
	float* DEPTH_BUFFER = new float[RASTER_SIZE];
	constexpr float NEAR_PLANE = 0.1f;
	constexpr float FAR_PLANE = 100.0f;


	// Math Constants
	constexpr float PIE = 3.141592653589793f;

	// Standard Colors
	constexpr unsigned int WHITE_BGRA = 0xFFFFFFFF;
	constexpr unsigned int WHITE_ARGB = 0xFFFFFFFF;
	constexpr unsigned int BLACK_ARGB = 0x00000000;
	constexpr unsigned int GREEN_ARGB = 0x0000FF00;
	constexpr unsigned int RED_ARGB = 0xFFFF0000;
	constexpr unsigned int BLUE_ARGB = 0xFF0000FF;
	constexpr unsigned int YELLOW_ARGB = 0xFFFFFF00;
	constexpr unsigned int CYAN_ARGB = 0xFF00FFFF;
	constexpr unsigned int MAGENTA_ARGB = 0xFFFF00FF;
	constexpr unsigned int ORANGE_ARGB = 0xFFFFA500;
	constexpr unsigned int PURPLE_ARGB = 0xFF800080;
	constexpr unsigned int GRAY_ARGB = 0xFF808080;
	constexpr unsigned int BROWN_ARGB = 0xFFA52A2A;
	constexpr unsigned int PINK_ARGB = 0xFFFFC0CB;
	constexpr unsigned int NAVY_BLUE_ARGB = 0xFF000050;
	constexpr unsigned int NAVY_BLUE_DARKER_ARGB = 0xFF000030;

}

#endif

#ifndef IUVO_COLORS
#define IUVO_COLORS

#include "IuvoDefines.h"

namespace IuvoColors {
	using namespace IuvoDefines;

	// raster color format
	struct ARGB_Color {
		unsigned int color;
		ARGB_Color(unsigned int _color = WHITE_ARGB) {
			color = _color;
		}
	};

	// source color format
	struct BGRA_Color {
		unsigned int color;
		BGRA_Color(unsigned int _color = WHITE_BGRA) {
			color = _color;
		}
	};


	ARGB_Color BGRA_To_ARGB(BGRA_Color BGRA_color) {
		// shift the components to the correct spot
		unsigned int oldColor = BGRA_color.color;

		unsigned int b = (oldColor & 0x000000FF);
		unsigned int g = (oldColor & 0x0000FF00) >> 8;
		unsigned int r = (oldColor & 0x00FF0000) >> 16;
		unsigned int a = (oldColor & 0xFF000000) >> 24;

		unsigned int newFormat = (a << 0) | (r << 8) | (g << 16) | (b << 24);
		ARGB_Color newColor(newFormat);

		return newColor;
	}

	struct Color {
		unsigned int rasterColor;
		bool containsAlpha = false;
		unsigned int red, blue, green, alpha;
		float alphaPercentage;
		std::string name = "";
		float lightRatio = 0.0f;

		Color(unsigned int _color = WHITE_ARGB, bool _isARGB = true, std::string _name = "") {
			red = 0;
			blue = 0;
			green = 0;
			alpha = 0;

			if (!_isARGB) {
				rasterColor = BGRA_To_ARGB(BGRA_Color(_color)).color;

			}
			else {
				rasterColor = _color;

			}

			alpha = ((rasterColor >> 24) & 0xFF);
			red = ((rasterColor >> 16) & 0xFF);
			green = ((rasterColor >> 8) & 0xFF);
			blue = (rasterColor & 0xFF);
			alphaPercentage = (float)(alpha / 255.0f);


			if (alpha != 0) {
				containsAlpha = true;
			}

			name = _name;
		}

		bool operator== (const Color& obj) {
			int i = 0;
			if (this->rasterColor == obj.rasterColor) {
				i += 1;
			}
			if (this->containsAlpha == obj.containsAlpha) {
				i += 1;
			}
			if (this->red == obj.red) {
				i += 1;
			}
			if (this->blue == obj.blue) {
				i += 1;
			}
			if (this->green == obj.green) {
				i += 1;
			}
			if (this->alpha == obj.alpha) {
				i += 1;
			}
			if (this->alphaPercentage == obj.alphaPercentage) {
				i += 1;
			}

			if (i == 7) {
				return true;
			}
			else {
				return false;
			}

		}
		void RecalculateMemberVariables() {
			alpha = ((rasterColor >> 24) & 0xFF);
			red = ((rasterColor >> 16) & 0xFF);
			green = ((rasterColor >> 8) & 0xFF);
			blue = (rasterColor & 0xFF);
			alphaPercentage = (float)(alpha / 255.0f);


			if (alpha != 0) {
				containsAlpha = true;
			}
		}
		void LogColor() {
			std::cout << "Start_Color_Log:\n" << std::endl;

			if (name != "") {
				std::cout << "Color Name: " << name << std::endl;
			}


			std::cout << "RasterColor: " << rasterColor << std::endl;
			std::cout << "Alpha: " << alpha << std::endl;
			std::cout << "Red: " << red << std::endl;
			std::cout << "Green: " << green << std::endl;
			std::cout << "Blue: " << blue << std::endl;
			std::cout << "AlphaPercentage: " << alphaPercentage << std::endl;
			std::cout << "Contains Alpha?: " << containsAlpha << std::endl;

			std::cout << std::endl;
			std::cout << "End_Color_Log:" << std::endl;

		}

	};

	Color LerpColor(const Color& primary, const Color& secondary, float ratio) {
		unsigned int newRed = (primary.red * secondary.red) * ratio;
		unsigned int newGreen = (primary.green * secondary.green) * ratio;
		unsigned int newBlue = (primary.blue * secondary.blue) * ratio;

		unsigned int newColor = (primary.alpha << 0) | (newRed << 8) | (newGreen << 16) | (newBlue << 24);

		return Color(newColor, true);
	}

	unsigned int BlendAlpha(Color bottomLayer, Color topLayer) {
		unsigned int newColor = 0;

		unsigned int newAlpha, newRed, newGreen, newBlue;



		//newAlpha = (unsigned int)(topLayer.alphaPercentage + bottomLayer.alphaPercentage * (1 - topLayer.alphaPercentage));
		newAlpha = (unsigned int)(1 - (1 - topLayer.alphaPercentage) * (1 - bottomLayer.alphaPercentage) * 255);
		newRed = (unsigned int)((bottomLayer.red - topLayer.red) * newAlpha + topLayer.red + 0.5f);
		newGreen = (unsigned int)((bottomLayer.green - topLayer.green) * newAlpha + topLayer.green + 0.5f);
		newBlue = (unsigned int)((bottomLayer.blue - topLayer.blue) * newAlpha + topLayer.blue + 0.5f);



		newColor = ((newAlpha << 24) | (newRed << 16) | (newGreen << 8) | (newBlue << 0));



		return newColor;
	}

	const Color colors[12] = { Color(WHITE_ARGB,true, "White"), Color(GREEN_ARGB,true, "Green"),
		Color(RED_ARGB,true, "Red"), Color(BLUE_ARGB,true,"Blue"), Color(YELLOW_ARGB,true, "Yellow"),
		Color(CYAN_ARGB,true,"Cyan"), Color(MAGENTA_ARGB,true,"Magenta"), Color(ORANGE_ARGB,true,"Orange"),
		Color(PURPLE_ARGB, true, "Purple"), Color(GRAY_ARGB, true, "Gray"), Color(BROWN_ARGB, true, "Brown"),
		Color(PINK_ARGB,true, "Pink")
	};

	const int colorsSize = 12;

	Color RandomColor() {
		int random = rand() % colorsSize;
		return colors[random];
	}



}


#endif
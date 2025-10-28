#ifndef IUVO_STRUCTS
#define IUVO_STRUCTS

#include "IuvoColors.h"

namespace IuvoStructs {

	using namespace IuvoColors;

	// struct for 2D coordinates
	struct RasterPosition {
		int x, y;

		RasterPosition() {
			x = 0;
			y = 0;
		}

		RasterPosition(int _x, int _y) {
			if (_x < RASTER_WIDTH) {
				x = _x;
			}
			else {
				x = RASTER_WIDTH - 1;
			}
			if (_y < RASTER_HEIGHT) {
				y = _y;
			}
			else {
				y = RASTER_HEIGHT - 1;
			}
		}

		bool IsInRange(RasterPosition minPos, RasterPosition maxPos) {
			if (x >= minPos.x && x <= maxPos.x && y >= minPos.y && y <= maxPos.y) {
				return true;
			}
			else { return false; }
		}
	};

	// struct to hold two sets of 2D coordinates
	// used to capture a "block" of pixels from a source
	struct RasterBlockImage {
		RasterPosition start, end;

		RasterBlockImage() {
			start = RasterPosition(0, 0);
			end = RasterPosition(1, 1);
		}
		RasterBlockImage(int x1, int y1, int x2, int y2) {
			start = RasterPosition(x1, y1);
			end = RasterPosition(x2, y2);
		}

		RasterBlockImage(RasterPosition topLeftCorner, RasterPosition bottomRightCorner) {
			start = topLeftCorner;
			end = bottomRightCorner;
		}
	};

	/// Float4 Struct
	struct Float4 {

		//struct's variables
		union
		{
			struct
			{
				float V[4];
			};

			struct
			{
				float x;
				float y;
				float z;
				float w;
			};
		};

		// default constructor
		Float4() {
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
			w = 0.0f;
		}
		// parametrized constructor
		Float4(float _x, float _y, float _z, float _w) {
			x = _x;
			y = _y;
			z = _z;
			w = _w;
		}
		void Log() {

			std::cout << "Float4 Log: \tX: " << x << "\t\tY: " << y << "\t\tZ: " << z << "\t\tW: " << w << std::endl;
		}
	};

	//struct to hold the UV cordinates for texturing the cube
	struct UV {

		//horizontal ratio
		float u_value;

		//vertical ratio
		float v_value;


		UV() {
			u_value = 0.25f;
			v_value = 0.25f;
		}

		UV(float horizontalValue, float verticalValue) {

			u_value = horizontalValue;
			v_value = verticalValue;
		}

	};

	struct Normal {
		float x, y, z;
		Normal() {
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
		}
		Normal(float _x, float _y, float _z) {
			x = _x;
			y = _y;
			z = _z;
		}
	};

	/// Vertex Struct
	struct Vertex {

		Float4 position;
		Color color;
		UV uv;
		Normal nrm;

		Vertex() {
			position = Float4();
			color = Color();
			uv = UV();
			nrm = Normal();
		}
		Vertex(Float4 pos, Color _color, UV _uv = UV()) {
			position = pos;
			color = _color;
			uv = _uv;
			nrm = Normal();
		}

		Vertex(float _x, float _y, float _z, float _w, Color _color = Color(WHITE_ARGB, true), UV _uv = UV()) {
			position.x = _x;
			position.y = _y;
			position.z = _z;
			position.w = _w;
			color = _color;
			uv = _uv;
			nrm = Normal();
		}
		Vertex(float _x, float _y, float _z, float _w, float horizontalRatio, float verticalRatio, Color _color = Color(WHITE_ARGB, true)) {
			position.x = _x;
			position.y = _y;
			position.z = _z;
			position.w = _w;
			color = _color;
			uv = UV(horizontalRatio, verticalRatio);
			nrm = Normal();
		}

		void LogData() {
			position.Log();
		}
	};


	/// 4x4 Matrix Struct
	struct Matrix4x4
	{
		// variables for the Matrix struct
		union
		{
			struct
			{
				float  V[16];
			};
			struct
			{
				float  xx;
				float  xy;
				float  xz;
				float  xw;
				float  yx;
				float  yy;
				float  yz;
				float  yw;
				float  zx;
				float  zy;
				float  zz;
				float  zw;
				float  wx;
				float  wy;
				float  wz;
				float  ww;
			};
			struct
			{
				Float4 AxisX;
				Float4 AxisY;
				Float4 AxisZ;
				Float4 AxisW;
			};
		};

		// default constructor initializes the matrix as an identity matrix
		Matrix4x4() {
			xx = 1.0f;  xy = 0.0f;  xz = 0.0f;  xw = 0.0f;
			yx = 0.0f;  yy = 1.0f;  yz = 0.0f;  yw = 0.0f;
			zx = 0.0f;  zy = 0.0f;  zz = 1.0f;  zw = 0.0f;
			wx = 0.0f;  wy = 0.0f;  wz = 0.0f;  ww = 1.0f;
		}
		// parametrized constructor that takes in and assigns four float4 variables to the members
		Matrix4x4(Float4 _AxisX, Float4 _AxisY, Float4 _AxisZ, Float4 _AxisW) {
			AxisX = _AxisX;
			AxisY = _AxisY;
			AxisZ = _AxisZ;
			AxisW = _AxisW;
		}
		void Log() {
			std::cout << "Matrix4x4 Log:\t";

			AxisX.Log();
			AxisY.Log();
			AxisZ.Log();
			AxisW.Log();
			std::cout << std::endl;
		}

	};

	struct Line {
		Vertex lineStart, lineEnd;
		float distance;

		Line() {
			lineStart = Vertex();
			lineEnd = Vertex();
			distance = CalculateDistance();
		}

		Line(Vertex start, Vertex end) {
			lineStart = start;
			lineEnd = end;
			distance = CalculateDistance();
		}

		float CalculateDistance() {
			if (lineStart.position.x > lineEnd.position.x) {
				Vertex temp = lineStart;
				lineStart = lineEnd;
				lineEnd = temp;
			}
			float distance = lineEnd.position.x - lineStart.position.x;

			return distance;
		}

		void LogData() {
			std::cout << "Line Log: \t";
			std::cout << "Start X: " << lineStart.position.x << "\tStart Y: " << lineStart.position.y << std::endl;
			std::cout << "End X: " << lineEnd.position.x << "\tEnd Y: " << lineEnd.position.y << std::endl;
			std::cout << "Distance : " << distance << "\n" << std::endl;
		}


	};

	struct BarycentricCoordinates {

		float alphaValue, betaValue, gammaValue;
		bool isInsideTriangle = false;

		BarycentricCoordinates() {
			alphaValue = 0.0f;
			betaValue = 0.0f;
			gammaValue = 0.0f;
		}

		BarycentricCoordinates(float _alpha, float _beta, float _gamma) {
			alphaValue = _alpha;
			betaValue = _beta;
			gammaValue = _gamma;
		}

		bool InsideTriangle() {
			float sum = (alphaValue + betaValue + gammaValue);

			if (alphaValue >= 0.0000001f && alphaValue < 0.9999999f && alphaValue != 0
				&& betaValue >= 0.0000001f && betaValue < 0.9999999f && betaValue != 0
				&& gammaValue >= 0.0000001f && gammaValue < 0.9999999f && gammaValue != 0) {
				isInsideTriangle = true;
			}
			return isInsideTriangle;
		}

		void LogData() {
			std::cout << "Barycentric Coordinates Log: \tAlpha: " << alphaValue <<
				"\tBeta: " << betaValue << "\tGamma: " << gammaValue << std::endl;
		}
	};

	struct Triangle {

		Vertex pointAlpha, pointBeta, pointGamma;
		float minX;
		float maxX;
		float minY;
		float maxY;
		RasterBlockImage totalFaceArea;

		// must be called for each point to test. stores no value unless recalculated based on a point
		BarycentricCoordinates barycentricCoords;


		Triangle() {
			pointAlpha = Vertex();
			pointBeta = Vertex();
			pointGamma = Vertex();
			CalculateMinAndMax();
			barycentricCoords = BarycentricCoordinates();
		}

		Triangle(Vertex a, Vertex b, Vertex c) {
			pointAlpha = a;
			pointBeta = b;
			pointGamma = c;
			CalculateMinAndMax();
			barycentricCoords = BarycentricCoordinates();
		}

		void CalculateMinAndMax() {
			// double check that each min and max is found
			minX = (std::min)({ pointAlpha.position.x, pointBeta.position.x, pointGamma.position.x });
			maxX = (std::max)({ pointAlpha.position.x, pointBeta.position.x, pointGamma.position.x });

			minY = (std::min)({ pointAlpha.position.y, pointBeta.position.y, pointGamma.position.y });
			maxY = (std::max)({ pointAlpha.position.y, pointBeta.position.y, pointGamma.position.y });

			totalFaceArea.start = RasterPosition(minX, minY);
			totalFaceArea.end = RasterPosition(maxX, maxY);
		}


		// Returns barycentric coordinates for a point (px, py) relative to the triangle
		BarycentricCoordinates CalculateBarycentricCoordinates(float px, float py) {
			float denominator = (pointBeta.position.y - pointGamma.position.y) * (pointAlpha.position.x - pointGamma.position.x) +
				(pointGamma.position.x - pointBeta.position.x) * (pointAlpha.position.y - pointGamma.position.y);

			// Calculate the barycentric coordinates for the point (px, py)
			float alpha = ((pointBeta.position.y - pointGamma.position.y) * (px - pointGamma.position.x) +
				(pointGamma.position.x - pointBeta.position.x) * (py - pointGamma.position.y)) / denominator;

			float beta = ((pointGamma.position.y - pointAlpha.position.y) * (px - pointGamma.position.x) +
				(pointAlpha.position.x - pointGamma.position.x) * (py - pointGamma.position.y)) / denominator;

			float gamma = 1.0f - alpha - beta;

			BarycentricCoordinates coords = BarycentricCoordinates(alpha, beta, gamma);
			return coords;
		}

		const void LogData() {
			pointAlpha.position.Log();
			pointBeta.position.Log();
			pointGamma.position.Log();

			std::cout << std::endl;
			std::cout << "Start X: " << totalFaceArea.start.x << "\tStart Y: " << totalFaceArea.start.y << std::endl;
			std::cout << "Start X: " << minX << "\tStart Y: " << minY << std::endl;
			std::cout << "End X: " << totalFaceArea.end.x << "\tEnd Y: " << totalFaceArea.end.y << std::endl;
			std::cout << "End X: " << maxX << "\tEnd Y: " << maxY << std::endl;

			std::cout << std::endl;

			barycentricCoords.LogData();

		}

	};

}

#endif
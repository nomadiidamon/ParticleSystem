#ifndef IUVO_RASTER
#define IUVO_RASTER

#include "IuvoMath.h"

namespace IuvoRaster {
	using namespace IuvoMath;

	/// This beginning section is for necessary 3D Cube components that shall remain constant

// Cube Vertices
	const Vertex top_frontLeft(-0.25f, 0.25f, 0.25f, 1.0f, RandomColor());
	const Vertex top_frontRight(0.25f, 0.25f, 0.25f, 1.0f, RandomColor());
	const Vertex top_backLeft(-0.25f, 0.25f, -0.25f, 1.0f, RandomColor());
	const Vertex top_backRight(0.25f, 0.25f, -0.25f, 1.0f, RandomColor());
	const Vertex bottom_frontLeft(-0.25f, -0.25f, 0.25f, 1.0f, RandomColor());
	const Vertex bottom_frontRight(0.25f, -0.25f, 0.25f, 1.0f, RandomColor());
	const Vertex bottom_backLeft(-0.25f, -0.25f, -0.25f, 1.0f, RandomColor());
	const Vertex bottom_backRight(0.25f, -0.25f, -0.25f, 1.0f, RandomColor());
	// array to hold all cube vertices for faster calculations
	const Vertex cubeVertices[8] = {
		top_frontLeft, top_frontRight, top_backLeft, top_backRight,
		bottom_frontLeft, bottom_frontRight, bottom_backLeft, bottom_backRight
	};



	//Front face triangles
	Triangle frontFace_Right(top_frontLeft, top_frontRight, bottom_frontRight);
	Triangle frontFace_Left(top_frontLeft, bottom_frontLeft, bottom_frontRight);
	// Top face triangles
	Triangle topFace_Right(top_frontRight, top_backRight, top_frontLeft);
	Triangle topFace_Left(top_backLeft, top_backRight, top_frontLeft);
	// Back face triangles
	Triangle backFace_Right(bottom_backRight, top_backRight, bottom_backLeft);
	Triangle backFace_Left(top_backLeft, top_backRight, bottom_backLeft);
	//Right face triangles
	Triangle rightFace_Right(top_frontRight, top_backRight, bottom_backRight);
	Triangle rightFace_Left(top_frontRight, bottom_frontRight, bottom_backRight);
	// Left face triangles
	Triangle leftFace_Right(top_frontLeft, top_backLeft, bottom_backLeft);
	Triangle leftFace_Left(top_frontLeft, bottom_frontLeft, bottom_backLeft);
	// bottom face triangles
	Triangle bottomFace_Right(bottom_backRight, bottom_frontRight, bottom_backLeft);
	Triangle bottomFace_Left(bottom_backLeft, bottom_frontRight, bottom_frontLeft);
	// array to hold the cube's triangles
	Triangle cubeTriangles[12] = {
		frontFace_Right, frontFace_Left, topFace_Right, topFace_Left, backFace_Right, backFace_Left,
		rightFace_Right, rightFace_Left, leftFace_Right, leftFace_Left, bottomFace_Right, bottomFace_Left
	};


	// cube matrices
	const Matrix4x4 cubeMatrix = IdentityMatrix();
	const Matrix4x4 cubeTranslationMatrix = TranslationMatrix(0, 0.25, 0);
	const Matrix4x4 translatedCubeMatrix = MultiplyMatrixByMatrix(cubeMatrix, cubeTranslationMatrix);
	const Matrix4x4 cubeRotationMatrix = MakeARotationY(CUBE_ROT_SPEED);
	const Matrix4x4 finalCubeMatrix = MultiplyMatrixByMatrix(translatedCubeMatrix, cubeRotationMatrix);

	// viewport matrices
	const Matrix4x4 viewMatrix = IdentityMatrix();
	const Matrix4x4 viewTranslationMatrix = TranslationMatrix(0, 10, -3);
	const Matrix4x4 viewRotationMatrix = MakeARotationX(-25);
	const Matrix4x4 viewTranslatedMatrix = MultiplyMatrixByMatrix(viewMatrix, viewTranslationMatrix);
	const Matrix4x4 viewRotatedMatrix = MultiplyMatrixByMatrix(viewMatrix, viewRotationMatrix);
	const Matrix4x4 rotatedViewMatrix = MultiplyMatrixByMatrix(viewTranslatedMatrix, viewRotatedMatrix);

	//zoom settings
	const Matrix4x4 zoomViewTranslationMatrix = TranslationMatrix(0, 0.25f, -7.5);
	const Matrix4x4 zoomViewRotationMatrix = MakeARotationX(-45);
	const Matrix4x4 zoomViewport = MultiplyMatrixByMatrix(IdentityMatrix(), zoomViewTranslationMatrix);
	const Matrix4x4 zoomViewportSettings = MultiplyMatrixByMatrix(zoomViewport, zoomViewRotationMatrix);

	// Projection Matrix
	float FOV_y = 90.0f * (PIE / 180);
	Matrix4x4 projectionMatrix = MakePerspectiveMatrix(FOV_y, ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);

	/// This is the end of the necessary const assignment components

	/// lighting information
	const Vertex directionalLight(Float4(-0.577f, -0.577f, 0.577f, 1.0f), 0xFFC0C0F0, UV());
	const Vertex pointLight(Float4(-1.0f, 0.5f, 1.0f, 1.0f), 0xFFFFFF00, UV());
	float directionalLightRatio = 0.0f;


	/// Shaders section from PPT slides	***START***

	// The active vertex shader. Modifies an incoming vertex. Pre-Rasterization. 
	static void (*VertexShader)(Vertex&) = 0;
	// The active pixel shader. Modifies an outgoing pixel. Post-Rasterization.
	static void (*PixelShader)(Color&, UV&) = 0;

	// All Shader Variables (Always Pre-fixed by “SV_”)
	static Matrix4x4 SV_WorldMatrixCube = IdentityMatrix();
	static Matrix4x4 SV_WorldMatrixGrid = IdentityMatrix();
	static Matrix4x4 SV_ViewMatrix = IdentityMatrix();
	static Matrix4x4 SV_ProjectionMatrix = IdentityMatrix();

	// Various custom vertex and pixel shaders, (Pre-fixed by “VS_” & “PS_”)
	// Can be swapped using above function pointers as needed for flexibility.

	// Applys the current world matrix to all vertices, if the object should be spun
	static void VS_SpinMe(Vertex& multiplyMe)
	{
		Vertex v = multiplyMe;
		v.uv = multiplyMe.uv;
		SV_WorldMatrixCube = MultiplyMatrixByMatrix(SV_WorldMatrixCube, cubeRotationMatrix);

		v = MultiplyMatrixByVertex(SV_WorldMatrixCube, v);
		v = MultiplyMatrixByVertex(SV_ViewMatrix, v);
		v = MultiplyMatrixByVertex(SV_ProjectionMatrix, v);
		v = PerspectiveDivide(v);

		multiplyMe = v;
	}

	// Applys the current world matrix to all vertices, if the object should remain stationary
	static void VS_Stationary(Vertex& multiplyMe)
	{

		Vertex v = multiplyMe;
		SV_WorldMatrixGrid = IdentityMatrix();

		v = MultiplyMatrixByVertex(SV_WorldMatrixGrid, v);
		v = MultiplyMatrixByVertex(SV_ViewMatrix, v);
		v = MultiplyMatrixByVertex(SV_ProjectionMatrix, v);
		v = PerspectiveDivide(v);

		multiplyMe = v;
	}



	// Basic pixel shader that changes the passed in color to white
	static void PS_White(Color& makeWhite, UV& _uv)
	{
		makeWhite.rasterColor = WHITE_ARGB;
		makeWhite.RecalculateMemberVariables();
	}

	// Basic pixel shader that changes the passed in color to black
	static void PS_Black(Color& makeBlack, UV& _uv)
	{
		makeBlack.rasterColor = BLACK_ARGB;
		makeBlack.RecalculateMemberVariables();
	}

	// Basic pixel shader that changes the passed in color to green
	static void PS_Green(Color& makeGreen, UV& _uv)
	{
		makeGreen.rasterColor = GREEN_ARGB;
		makeGreen.RecalculateMemberVariables();
	}

	// Basic pixel shader that changes the passed in color to red
	static void PS_Red(Color& makeRed, UV& _uv)
	{
		makeRed.rasterColor = RED_ARGB;
		makeRed.RecalculateMemberVariables();
	}

	// Basic pixel shader that changes the passed in color to yellow
	static void PS_Yellow(Color& makeYellow, UV& _uv)
	{
		makeYellow.rasterColor = YELLOW_ARGB;
		makeYellow.RecalculateMemberVariables();
	}

	// Basic pixel shader that changes the passed in color to blue
	static void PS_Blue(Color& makeBlue, UV& _uv)
	{
		makeBlue.rasterColor = BLUE_ARGB;
		makeBlue.RecalculateMemberVariables();
	}

	// Basic pixel shader that changes the passed in color to purple
	static void PS_Purple(Color& makePurple, UV& _uv)
	{
		makePurple.rasterColor = PURPLE_ARGB;
		makePurple.RecalculateMemberVariables();
	}

	// Basic pixel shader that changes the passed in color to pink
	static void PS_Pink(Color& makePink, UV& _uv)
	{
		makePink.rasterColor = PINK_ARGB;
		makePink.RecalculateMemberVariables();
	}

	// Basic pixel shader that changes the passed in color to brown
	static void PS_Brown(Color& makeBrown, UV& _uv)
	{
		makeBrown.rasterColor = BROWN_ARGB;
		makeBrown.RecalculateMemberVariables();
	}

	// Basic pixel shader that changes the passed in color to purple
	static void PS_Magenta(Color& makeMagenta, UV& _uv)
	{
		makeMagenta.rasterColor = MAGENTA_ARGB;
		makeMagenta.RecalculateMemberVariables();
	}

	// Basic pixel shader that changes the passed in color to purple
	static void PS_Cyan(Color& makeCyan, UV& _uv)
	{
		makeCyan.rasterColor = CYAN_ARGB;
		makeCyan.RecalculateMemberVariables();
	}

	/// Shaders section from PPT slides	***END***




	void ClearColorBuffer(Color _color) {
		for (int i = 0; i < RASTER_SIZE; i++) {
			DESTINATION_ARRAY[i] = _color.rasterColor;
		}
	}

	int Convert2D_To_1D(RasterPosition point, int width) {
		int convertedIndex = (point.y * width) + point.x;
		return convertedIndex;
	}

	void PaintPixel(int _index, Color _color) {
		if (_index >= RASTER_SIZE) {
			std::cout << "Given position is out of bounds. Ignoring plot command.\n" << std::endl;
			return;
		}

		if (_index >= 0 && _index <= RASTER_SIZE) {
			DESTINATION_ARRAY[_index] = _color.rasterColor;
		}
	}

	void DrawPixel(RasterPosition pos, Color _color, UV _uv = UV(0.5f, 0.5f)) {
		int index = Convert2D_To_1D(pos, RASTER_WIDTH);
		if (PixelShader) {
			PixelShader(_color, _uv);
		}
		PaintPixel(index, _color.rasterColor);
	}

	void DrawLineBrensham(RasterPosition start, RasterPosition end, Color _color) {
		int startX = start.x;
		int startY = start.y;
		int endX = end.x;
		int endY = end.y;

		int deltaX = abs(endX - startX);
		int deltaY = abs(endY - startY);
		int error = 0;

		// Determine the direction of movement in the x and y axes
		int stepX;
		if (startX < endX) {
			stepX = 1;
		}
		else {
			stepX = -1;
		}

		int stepY;
		if (startY < endY) {
			stepY = 1;
		}
		else {
			stepY = -1;
		}

		// Verticle
		if (deltaX == 0) {
			for (int y = startY; y <= endY; y++) {
				DrawPixel(RasterPosition(startX, y), _color);
			}
			return;
		}

		// Horizontal
		if (deltaY == 0) {
			for (int x = startX; x <= endX; x++) {
				DrawPixel(RasterPosition(x, startY), _color);
			}
			return;
		}

		// Calculate the line using Brensham step algorithm
		if (deltaX >= deltaY) {
			// Line is more horizontal
			for (int x = startX; x != endX; x += stepX) {
				DrawPixel(RasterPosition(x, startY), _color);
				error += deltaY;
				if (2 * error >= deltaX) {
					startY += stepY;
					error -= deltaX;
				}
			}
		}
		else {
			// Line is more vertical
			for (int y = startY; y != endY; y += stepY) {
				DrawPixel(RasterPosition(startX, y), _color);
				error += deltaX;
				if (2 * error >= deltaY) {
					startX += stepX;
					error -= deltaY;
				}
			}
		}
	}

	void DrawLineParametric(RasterPosition start, RasterPosition end, Color _color) {

		if (start.x > end.x) {
			RasterPosition temp = start;

			start = end;
			end = temp;
		}

		int startX = start.x;
		int startY = start.y;
		int endX = end.x;
		int endY = end.y;



		int totalDeltaX = (endX - startX);
		int totalDeltaY = (endY - startY);

		// Handle vertical lines
		if (totalDeltaX == 0) {
			for (int y = startY; y <= endY; y++) {
				DrawPixel(RasterPosition(startX, y), _color);
			}
			return;
		}
		// Handle horizontal lines
		else if (totalDeltaY == 0) {
			for (int x = startX; x <= endX; x++) {
				DrawPixel(RasterPosition(x, startY), _color);
			}
			return;
		}

		// For nearly vertical or nearly horizontal lines, use Bresenham's algorithm
		if (abs(totalDeltaX) > abs(totalDeltaY)) {
			// The line is closer to horizontal, loop through based on the X values
			float ratio = 0.0f;
			if (totalDeltaX > 0) {
				for (int x = startX; x <= endX; x++) {
					ratio = float(x - startX) / totalDeltaX;
					int y = int(startY + ratio * totalDeltaY + 0.5f);
					DrawPixel(RasterPosition(x, y), _color);
				}
			}
			else {
				for (int x = startX; x >= endX; x--) {
					ratio = float(x - startX) / totalDeltaX;
					int y = int(startY + ratio * totalDeltaY + 0.5f);
					DrawPixel(RasterPosition(x, y), _color);
				}
			}
		}
		else {
			// The line is closer to vertical, loop through based on the Y values
			float ratio = 0.0f;
			if (totalDeltaY > 0) {
				for (int y = startY; y <= endY; y++) {
					ratio = float(y - startY) / totalDeltaY;
					int x = int(startX + ratio * totalDeltaX + 0.5f);
					DrawPixel(RasterPosition(x, y), _color);
				}
			}
			else {
				for (int y = startY; y >= endY; y--) {
					ratio = float(y - startY) / totalDeltaY;
					int x = int(startX + ratio * totalDeltaX + 0.5f);
					DrawPixel(RasterPosition(x, y), _color);
				}
			}

		}
	}



	namespace IuvoRasterBLIT {

		void BLIT(RasterBlockImage blockToCopy, RasterPosition posToCopyTo, int sourceWidth, const unsigned int* source, bool blendAlpha) {

			for (int y = blockToCopy.start.y; y < blockToCopy.end.y; y++) {

				for (int x = blockToCopy.start.x; x < blockToCopy.end.x; x++) {


					if (blendAlpha) {
						// get the current destination color
						int destinationIndex = Convert2D_To_1D(RasterPosition(x + posToCopyTo.x, y + posToCopyTo.y), sourceWidth);

						Color destinationColor = DESTINATION_ARRAY[destinationIndex];


						//get the color to replace the destination index
						int sourceIndex = Convert2D_To_1D(RasterPosition(x, y), sourceWidth);
						BGRA_Color _sourceColor(source[sourceIndex]);
						Color sourceColor(_sourceColor.color, false);




						if (!sourceColor.containsAlpha) {

							if (sourceColor.alpha == 255) {
								DrawPixel(RasterPosition(x + posToCopyTo.x, y + posToCopyTo.y), destinationColor);
							}
							else {

								Color newColor(IuvoColors::BlendAlpha(destinationColor, sourceColor), true);
								DrawPixel(RasterPosition(x + posToCopyTo.x, y + posToCopyTo.y), newColor);

							}

						}
						else {
							DrawPixel(RasterPosition(x + posToCopyTo.x, y + posToCopyTo.y), sourceColor);
						}
					}
					else {
						int sourceIndex = Convert2D_To_1D(RasterPosition(x, y), sourceWidth);

						BGRA_Color sourceColor(source[sourceIndex]);
						DrawPixel(RasterPosition(x + posToCopyTo.x, y + posToCopyTo.y), Color(sourceColor.color, false).rasterColor);
					}
				}
			}
		}

		RasterPosition RandomPositionForBLIT(int sourceHeight, int sourceWidth, RasterBlockImage toBLIT) {

			int width = toBLIT.end.x - toBLIT.start.x;
			int height = toBLIT.end.y - toBLIT.start.y;

			int maxX = RASTER_WIDTH - (width * 10);
			int maxY = RASTER_HEIGHT - (height * 10);

			int randX = rand() % maxX;
			int randY = rand() % maxY;

			return RasterPosition(randX, randY);

		}
	};

	Vertex NDC_to_Screen(Vertex inVertex)
	{
		Vertex outVertex = inVertex;
		outVertex.position.x = (1 + outVertex.position.x) * RASTER_WIDTH / 2;
		outVertex.position.y = (1 - outVertex.position.y) * RASTER_HEIGHT / 2;
		return outVertex;
	}

	void DrawLine3D(Vertex _start, Vertex _end, Color _color)
	{
		Vertex copy_start = _start;
		Vertex copy_end = _end;

		if (VertexShader) {
			VertexShader(copy_start);
			VertexShader(copy_end);
		}

		Vertex start = NDC_to_Screen(copy_start);
		Vertex end = NDC_to_Screen(copy_end);



		RasterPosition startPos((int)start.position.x, (int)start.position.y);
		RasterPosition endPos((int)end.position.x, (int)end.position.y);


		DrawLineParametric(startPos, endPos, _color);

	}

	void DrawLine3D(Line line, Color _color)
	{
		Vertex copy_start = line.lineStart;
		Vertex copy_end = line.lineEnd;

		if (VertexShader) {
			VertexShader(copy_start);
			VertexShader(copy_end);
		}

		Vertex start = NDC_to_Screen(copy_start);
		Vertex end = NDC_to_Screen(copy_end);



		RasterPosition startPos((int)start.position.x, (int)start.position.y);
		RasterPosition endPos((int)end.position.x, (int)end.position.y);
		DrawLineParametric(startPos, endPos, _color);

	}

	void DrawPixel3D(Vertex vertex, Color color) {

		Vertex pixel = vertex;

		if (VertexShader) {
			VertexShader(pixel);
		}

		Vertex spot = NDC_to_Screen(pixel);

		// if the point is behind the camera frustrum, ignore it
		if (spot.position.z < NEAR_PLANE) {
			return;
		}

		if (spot.position.w < 0.0f) {
			return;
		}
		if (spot.position.x < -1.0f) {
			return;
		}
		if (spot.position.y < -1.0f) {
			return;
		}


		RasterPosition postionToPaint((int)spot.position.x, (int)spot.position.y);

		DrawPixel(postionToPaint, spot.color);

	}

	void FillTriangleBrute3D(Triangle triangle, Color color, bool addDepth) {

		Vertex A, B, C;
		A = triangle.pointAlpha;
		B = triangle.pointBeta;
		C = triangle.pointGamma;

		if (VertexShader) {
			VertexShader(A);
			VertexShader(B);
			VertexShader(C);
		}

		Vertex newA, newB, newC;
		newA = NDC_to_Screen(A);
		newB = NDC_to_Screen(B);
		newC = NDC_to_Screen(C);

		Triangle newTriangle(newA, newB, newC);

		int currentX = 0;
		int currentY = 0;

		for (currentY = 0; currentY < RASTER_HEIGHT; currentY++) {

			for (currentX = 0; currentX < RASTER_WIDTH; currentX++) {

				BarycentricCoordinates coords = newTriangle.CalculateBarycentricCoordinates((float)currentX, (float)currentY);
				float sum = (coords.alphaValue + coords.betaValue + coords.gammaValue);
				if (coords.InsideTriangle()) {
					if (addDepth) {
						// set up the depth buffer value
						float newZValue = ((newTriangle.pointAlpha.position.z * coords.alphaValue) + (newTriangle.pointBeta.position.z * coords.betaValue) + (newTriangle.pointGamma.position.z * coords.gammaValue));
						int index = Convert2D_To_1D(RasterPosition(currentX, currentY), RASTER_WIDTH);

						float currentZValue = DEPTH_BUFFER[index];

						if (newZValue < currentZValue) {
							// emplace the new depth value
							DEPTH_BUFFER[index] = newZValue;

							// draw the new pixel color at the given point
							DrawPixel(RasterPosition(currentX, currentY), color);
						}
					}
					else {
						DrawPixel(RasterPosition(currentX, currentY), color);
					}
				}
			}
		}

	}


	void FillTriangleBetterBrute3D(Triangle triangle, Color color, bool addDepth) {

		triangle.CalculateMinAndMax();

		Vertex A, B, C;
		A = triangle.pointAlpha;
		B = triangle.pointBeta;
		C = triangle.pointGamma;

		if (VertexShader) {
			VertexShader(A);
			VertexShader(B);
			VertexShader(C);
		}

		Vertex newA, newB, newC;
		newA = NDC_to_Screen(A);
		newB = NDC_to_Screen(B);
		newC = NDC_to_Screen(C);

		Triangle newTriangle(newA, newB, newC);
		newTriangle.CalculateMinAndMax();

		int currentX = 0;
		int currentY = 0;

		for (currentY = newTriangle.minY; currentY < newTriangle.maxY + 1; currentY++) {

			for (currentX = newTriangle.minX; currentX < newTriangle.maxX + 1; currentX++) {

				BarycentricCoordinates coords = newTriangle.CalculateBarycentricCoordinates((float)currentX, (float)currentY);
				if (coords.InsideTriangle()) {
					if (addDepth) {
						// set up the depth buffer value
						float newZValue = ((newTriangle.pointAlpha.position.z * coords.alphaValue) + (newTriangle.pointBeta.position.z * coords.betaValue) + (newTriangle.pointGamma.position.z * coords.gammaValue));
						int index = Convert2D_To_1D(RasterPosition(currentX, currentY), RASTER_WIDTH);

						float currentZValue = DEPTH_BUFFER[index];

						if (newZValue < currentZValue) {
							// emplace the new depth value
							DEPTH_BUFFER[index] = newZValue;

							// draw the new pixel color at the given point
							DrawPixel(RasterPosition(currentX, currentY), color);
						}
					}
					else {
						DrawPixel(RasterPosition(currentX, currentY), color);
					}


				}
			}
		}

	}

	void FillTriangleBetterBrute3DForTexturing(Triangle triangle, Color color, bool addDepth) {



		triangle.CalculateMinAndMax();

		Vertex A, B, C;
		A = triangle.pointAlpha;
		A.uv = triangle.pointAlpha.uv;
		B = triangle.pointBeta;
		B.uv = triangle.pointBeta.uv;
		C = triangle.pointGamma;
		C.uv = triangle.pointGamma.uv;

		if (VertexShader) {
			VertexShader(A);
			VertexShader(B);
			VertexShader(C);
		}

		Vertex newA, newB, newC;
		newA = NDC_to_Screen(A);
		newA.uv = A.uv;
		newB = NDC_to_Screen(B);
		newB.uv = B.uv;
		newC = NDC_to_Screen(C);
		newC.uv = C.uv;

		Triangle newTriangle(newA, newB, newC);
		newTriangle.CalculateMinAndMax();

		int currentX = 0;
		int currentY = 0;

		for (currentY = newTriangle.minY; currentY < newTriangle.maxY + 1; currentY++) {

			for (currentX = newTriangle.minX; currentX < newTriangle.maxX + 1; currentX++) {

				BarycentricCoordinates coords = newTriangle.CalculateBarycentricCoordinates((float)currentX, (float)currentY);
				if (coords.InsideTriangle()) {
					if (addDepth) {
						// set up the depth buffer value
						float newZValue = ((newTriangle.pointAlpha.position.z * coords.alphaValue) + (newTriangle.pointBeta.position.z * coords.betaValue) + (newTriangle.pointGamma.position.z * coords.gammaValue));
						unsigned int index = 0;
						index = Convert2D_To_1D(RasterPosition(currentX, currentY), RASTER_WIDTH);

						float newU = ((newTriangle.pointAlpha.uv.u_value * coords.alphaValue) + (newTriangle.pointBeta.uv.u_value * coords.betaValue) + (newTriangle.pointGamma.uv.u_value * coords.gammaValue));
						float newV = ((newTriangle.pointAlpha.uv.v_value * coords.alphaValue) + (newTriangle.pointBeta.uv.v_value * coords.betaValue) + (newTriangle.pointGamma.uv.v_value * coords.gammaValue));

						///
						///

						UV newUV(newU, newV);
						if (PixelShader) {
							PixelShader(color, newUV);
						}

						//if (index > 0 && index < RASTER_SIZE) {
						float currentZValue = DEPTH_BUFFER[index];

						if (newZValue < currentZValue) {
							// emplace the new depth value
							DEPTH_BUFFER[index] = newZValue;

							// draw the new pixel color at the given point
							PaintPixel(index, color);
						}

						//}






					}
				}
			}
		}

	}

	void DrawTriangle3D(Triangle triangle, Color color, bool addDepth) {
		FillTriangleBetterBrute3D(triangle, color, addDepth);
	}

	void DrawTriangle3DTextured(Triangle triangle, Color color, bool addDepth) {
		FillTriangleBetterBrute3DForTexturing(triangle, color, addDepth);
	}



	void ResetDepthBuffer() {
		for (int i = 0; i < RASTER_SIZE; i++) {
			DEPTH_BUFFER[i] = 1.0f;
		}
	}

	Vertex GetRandomVertexForStars(Color starColor) {

		float min = -1.0f;
		float max = 1.0f;



		float randX = min + (float)((rand() / ((float)RAND_MAX / (max - min))));
		float randY = min + (float)((rand() / ((float)RAND_MAX / (max - min))));
		float randZ = min + (float)((rand() / ((float)RAND_MAX / (max - min))));

		return Vertex(Float4(randX, randY, randZ, 1.0f), starColor);

	}

	void ScaleStar(Vertex& star) {
		Vertex vert = star;

		vert.position.x *= 50;
		vert.position.y *= 50;
		vert.position.z *= 50;
		//vert.position.w *= 50;

		star = vert;
	}

	void ScaleModel(Vertex& model) {
		Vertex vert = model;

		vert.position.x *= 0.1f;
		vert.position.y *= 0.1f;
		vert.position.z *= 0.1f;
		//vert.position.w *= 0.1f;

		model = vert;
	}

}


#endif

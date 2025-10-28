#ifndef IUVO_NUMBERS
#define IUVO_NUMBERS

#include <cstdlib>
#include <ctime>
#include <vector>

namespace IuvoNumbers {
	/// <summary>
	/// bool to check wether or not the seed has been initialized or not
	/// </summary>
	bool randHasBeenInitilized = false;
	/// <summary>
	/// Predetermined value to return when random number range is invalid
	/// </summary>
	int RANDOM_INT_ERROR = -1;
	/// <summary>
	/// Predetermined value to return when random number range is invalid
	/// </summary>
	float RANDOM_FLOAT_ERROR = -1.11f;


	/// <summary>
	/// Needs to be called before any calls to rand()
	/// </summary>
	void InitializeRandomSeed() {
		std::srand(std::time(NULL));
		randHasBeenInitilized = true;
	}

	/// <summary>
	/// Returns a random number within the given range. Ensure the seed has been initialized before hand
	/// </summary>
	/// <param name="MinRange"> is the minimum number for the random generation</param>
	/// <param name="MaxRange"> is the maximum number for the random generation</param>
	/// <returns> the randomized number</returns>
	int RandomInt(int MinRange, int MaxRange)
	{
		if (!randHasBeenInitilized) {
			InitializeRandomSeed();
		}

		if (MinRange == 0 && MaxRange == 0) {
			return rand();
		}
		if (MinRange >= MaxRange) {
			return RANDOM_INT_ERROR;
		}
		int range = MaxRange - MinRange;
		return (rand() % range) + MinRange;
	}

	/// <summary>
	/// Returns a random float number within the given range.
	/// </summary>
	/// <param name="MinRange"> is the minimum number for the random generation</param>
	/// <param name="MaxRange"> is the maximum number for the random generation</param>
	/// <returns> the randomized float</returns>
	float RandomFloat(float MinRange, float MaxRange)
	{
		if (!randHasBeenInitilized) {
			InitializeRandomSeed();
		}

		if (MinRange >= MaxRange) {
			return -1.0f;  // Error case
		}

		float range = MaxRange - MinRange;
		return ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * range) + MinRange;
	}

	/// <summary>
	/// Returns a random boolean value (true or false).
	/// </summary>
	/// <returns> true or false</returns>
	bool RandomBool()
	{
		if (!randHasBeenInitilized) {
			InitializeRandomSeed();
		}

		return rand() % 2;  // 50% chance for true or false
	}

	/// <summary>
	/// Returns a random element from a given list/array.
	/// </summary>
	/// <param name="list">Array of integers to choose from</param>
	/// <param name="size">The size of the array</param>
	/// <returns> A random value from the list</returns>
	int RandomFromList(int list[], int size)
	{
		if (!randHasBeenInitilized) {
			InitializeRandomSeed();
		}

		if (size <= 0) {
			return RANDOM_INT_ERROR;
		}

		int index = rand() % size;
		return list[index];
	}

	/// <summary>
	/// Returns a random element from a given list/array.
	/// </summary>
	/// <param name="list">Array of integers to choose from</param>
	/// <param name="size">The size of the array</param>
	/// <returns> A random value from the list</returns>
	int RandomFromList(std::vector<int> list, int size)
	{
		if (!randHasBeenInitilized) {
			InitializeRandomSeed();
		}

		if (size <= 0) {
			return RANDOM_INT_ERROR;
		}

		int index = rand() % size;
		return list[index];
	}

	/// <summary>
	/// Returns a random element from a given list/array.
	/// </summary>
	/// <param name="list">Array of floats to choose from</param>
	/// <param name="size">The size of the array</param>
	/// <returns> A random value from the list</returns>
	float RandomFromList(float list[], int size)
	{
		if (!randHasBeenInitilized) {
			InitializeRandomSeed();
		}

		if (size <= 0) {
			return RANDOM_INT_ERROR;
		}

		int index = rand() % size;
		return list[index];
	}

	/// <summary>
	/// Returns a random element from a given list/array.
	/// </summary>
	/// <param name="list">Vector of floats to choose from</param>
	/// <returns> A random value from the list</returns>
	float RandomFromList(std::vector<float> list)
	{
		if (!randHasBeenInitilized) {
			InitializeRandomSeed();
		}

		if (list.size() <= 0) {
			return RANDOM_INT_ERROR;
		}

		int index = rand() % list.size();
		return list[index];
	}

	/// <summary>
	/// Clamps a number to a given range. If the number is less than the min, it returns the min. If greater than max, it returns the max.
	/// </summary>
	/// <param name="value">The number to clamp</param>
	/// <param name="minValue">The minimum allowed value</param>
	/// <param name="maxValue">The maximum allowed value</param>
	/// <returns> The clamped value</returns>
	int Clamp(int value, int minValue, int maxValue)
	{
		if (value < minValue) {
			return minValue;
		}
		else if (value > maxValue) {
			return maxValue;
		}
		return value;
	}

	/// <summary>
	/// Clamps a long value between a minimum and maximum value.
	/// </summary>
	/// <param name="value">The value to clamp.</param>
	/// <param name="min">The minimum allowed value.</param>
	/// <param name="max">The maximum allowed value.</param>
	/// <returns>The clamped value.</returns>
	long Clamp(long value, long min, long max)
	{
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}

	/// <summary>
	/// Clamps a float value between a minimum and maximum value.
	/// </summary>
	/// <param name="value">The value to clamp.</param>
	/// <param name="min">The minimum allowed value.</param>
	/// <param name="max">The maximum allowed value.</param>
	/// <returns>The clamped value.</returns>
	float Clamp(float value, float min, float max)
	{
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}

	/// <summary>
	/// Clamps a double value between a minimum and maximum value.
	/// </summary>
	/// <param name="value">The value to clamp.</param>
	/// <param name="min">The minimum allowed value.</param>
	/// <param name="max">The maximum allowed value.</param>
	/// <returns>The clamped value.</returns>
	double Clamp(double value, double min, double max)
	{
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}


	/// <summary>
	/// Returns whether a number is prime.
	/// </summary>
	/// <param name="n">The number to check</param>
	/// <returns> true if the number is prime, false otherwise</returns>
	bool IsPrime(int n)
	{
		if (n <= 1) return false;

		for (int i = 2; i * i <= n; ++i) {
			if (n % i == 0) {
				return false;
			}
		}
		return true;
	}

	/// <summary>
	/// Returns the greatest common divisor of two numbers using the Euclidean algorithm.
	/// </summary>
	/// <param name="a">First number</param>
	/// <param name="b">Second number</param>
	/// <returns> The greatest common divisor (GCD)</returns>
	int GCD(int a, int b)
	{
		while (b != 0) {
			int temp = b;
			b = a % b;
			a = temp;
		}
		return a;
	}

	/// <summary>
	/// Returns the greatest common divisor of two long numbers using the Euclidean algorithm.
	/// </summary>
	/// <param name="a">First number</param>
	/// <param name="b">Second number</param>
	/// <returns> The greatest common divisor (GCD)</returns>
	long GCD(long a, long b)
	{
		while (b != 0) {
			long temp = b;
			b = a % b;
			a = temp;
		}
		return a;
	}

	/// <summary>
	/// Returns the greatest common divisor of two floating-point numbers (rounded to nearest integers) using the Euclidean algorithm.
	/// </summary>
	/// <param name="a">First number</param>
	/// <param name="b">Second number</param>
	/// <returns> The greatest common divisor (GCD)</returns>
	int GCD(float a, float b)
	{
		// Round to nearest integer
		return GCD(static_cast<int>(round(a)), static_cast<int>(round(b)));
	}

	/// <summary>
	/// Returns the greatest common divisor of two double numbers (rounded to nearest integers) using the Euclidean algorithm.
	/// </summary>
	/// <param name="a">First number</param>
	/// <param name="b">Second number</param>
	/// <returns> The greatest common divisor (GCD)</returns>
	int GCD(double a, double b)
	{
		// Round to nearest integer
		return GCD(static_cast<int>(round(a)), static_cast<int>(round(b)));
	}



}


#endif


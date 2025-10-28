#ifndef IUVO_STRING_VECTOR
#define IUVO_STRING_VECTOR

#include "IuvoNumbers.h"
#include <vector>
#include <string>

namespace IuvoStringVector {

	/// <summary>
	/// Pulls a random string from a vector of strings
	/// </summary>
	/// <param name="toSelectFrom"> is the vector from which we will be taking the random value</param>
	/// <returns> a randomly selected string from the given vector</returns>
	std::string RandomString(const std::vector<std::string>& toSelectFrom) {
		int namesVectorSize = toSelectFrom.size();
		if (!IuvoNumbers::randHasBeenInitilized) {
			IuvoNumbers::InitializeRandomSeed();
		}

		int randomIndex = std::rand() % namesVectorSize;
		return toSelectFrom[randomIndex];
	}

	/// <summary>
	/// Checks a string against a vector of string to determine if the new string is unique or not
	/// </summary>
	/// <param name="words"> is the string vector that will be searched</param>
	/// <param name="toSearchFor"> is the string that will be looked for in the given vector</param>
	/// <returns> true if the value was not found, otherwise returns false</returns>
	bool IsUnique(const std::vector<std::string>& words, const std::string& toSearchFor) {
		return std::find(words.begin(), words.end(), toSearchFor) == words.end();
	}
}

#endif
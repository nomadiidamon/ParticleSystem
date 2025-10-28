#ifndef IUVO_NAMES
#define IUVO_NAMES

#include "IuvoStringVector.h"
#include <ctime>

namespace IuvoNames {

	// nomines is Latin for names
	const std::vector<std::string> nomines = {
		"Keon", "Tui", "Limak", "Adeline", "Isam",
		"Kamek","Namiko", "Vigi", "Durk", "Aguilar",
		"Khai", "Tyson", "Koda", "Vasu", "Jadon",
		"Rhea", "Yali", "Yalu", "Neveah", "Kaia",
		"Cara", "Daya", "Eileen", "Oba", "Semmi",
		"Anita", "Ainara", "Jerold", "Tobias", "Palafox",
		"Alcetas", "Karno","Grozdan", "Selasi", "Gerard",
		"Callum", "Arrun", "Maidan", "Zakaria",
		"Sankai", "Ziza", "Alarcon", "Aulus", "Drymadoria",
		"Drimadoria", "Lykianti", "Nazar", "Elnashar",
		"Aminita", "Oha", "Aoleon", "Javad", "Thiago",
		"Palafax", "Ogallala", "Menin", "Engedi", "Rosta",
		"Cairno", "Azgoah", "Saurr", "Merav", "Eknashar"
	};



	/// <summary>
	/// Fills a given vector with randomly selected strings from another vector. Runs recursively until the desired number is met
	/// </summary>
	/// <param name="numberOfNames"> is the number of unique names needed to successfully exit the function</param>
	/// <param name="toReturn"> is the vector that the unique strings will be stored in</param>
	/// <param name="toSelectFrom"> is the vector that random strings will be chosen from</param>
	void GenerateUniqueName(int numberOfNames, std::vector<std::string>& toReturn, const std::vector<std::string>& toSelectFrom) {
		if (toReturn.size() == numberOfNames) {
			return;
		}

		std::string chosen = IuvoStringVector::RandomString(toSelectFrom);

		if (IuvoStringVector::IsUnique(toReturn, chosen)) {
			toReturn.push_back(chosen);
		}

		GenerateUniqueName(numberOfNames, toReturn, toSelectFrom);

	}

	/// <summary>
	/// Randomly selects a specefic number of strings from the provided vector
	/// </summary>
	/// <param name="numberOfNames"> is the desired size of the vector to return</param>
	/// <param name="toSelectFrom"> is the vector that will be randomly chosen from</param>
	/// <returns> a vector holding the number of desired random names</returns>
	std::vector<std::string> RandomName(int numberOfNames, const std::vector<std::string>& toSelectFrom) {		
		std::vector<std::string> toReturn;
		toReturn.clear();
		toReturn.reserve(numberOfNames);

		GenerateUniqueName(numberOfNames, toReturn, toSelectFrom);

		return toReturn;
	}
}

#endif
#ifndef IUVO_INPUT
#define IUVO_INPUT


namespace IuvoInput {

	bool WipeAndFlush()
	{
		std::cout << "Input failed, try again.";
		std::cin.ignore(INT_MAX, '\n');
		std::cin.clear();
		return 1;
	}

	bool Flush()
	{
		std::cin.ignore(INT_MAX, '\n');
		std::cin.clear();
		return 1;
	}

	int GetValidatedInt(const char* message, int MinRange, int MaxRange)
	{
		std::cout << message;
		int number;
		std::cin >> number;
		if (std::cin.fail()) {
			WipeAndFlush();
			GetValidatedInt(message, MinRange, MaxRange);
		}
		Flush();
		if (MinRange == 0 && MaxRange == 0) {
			return number;
		}
		else {
			if (MinRange < number && number < MaxRange) {
				return number;
			}
			else {
				WipeAndFlush();
				GetValidatedInt(message, MinRange, MaxRange);
			}
		}
	}

	char* GetValidatedString_255(const char* promptMessage)
	{
		std::cout << promptMessage;

		char* ref = new char[255];
		std::cin.getline(ref, 255);
		if ((unsigned)strlen(ref) < 255) {
			return ref;
		}
		else {
			delete[] ref;
			Flush();
			std::cout << "Input was not within the valid range, try again." << std::endl;
			GetValidatedString_255(promptMessage);
		}
	}

	char* String_255()
	{
		char* ref = new char[255];
		std::cin.getline(ref, 255);
		return ref;
	}



}

#endif 
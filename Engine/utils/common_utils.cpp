#include "common_utils.h"


// Clears input stream to prevent errors with its buffer
void StreamUtils::clearInputStream() {
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void StreamUtils::softClearInputStream() {

	if (std::cin.fail() || std::cin.bad()) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	else 
		while (std::cin.peek() == ' ' || std::cin.peek() == '\t')
			std::cin.ignore(1);
		
}

// Input unsigned int in the safety, especially for proj_tests.h
bool StreamUtils::safeInputUInt(unsigned int& val) {
	std::string input;
	std::getline(std::cin, input);

	if (input.empty())
		return false;

	for (char c : input)
		if (!std::isdigit(c))
			return false;

	try {
		unsigned long temp = std::stoul(input);
		if (temp > std::numeric_limits<unsigned int>::max())
			return false;

		val = static_cast<unsigned int>(temp);
		return true;
	}
	catch (const std::exception&) {
		return false;
	}

}
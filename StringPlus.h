#pragma once

#include <string>

#include <msclr\marshal_cppstd.h>  //  needed for string conversions

//using namespace std;

class StringPlus {
public:
	// trims/removes leading spaces, newlines, and tabs
	static std::string ltrim(const std::string& inString) {
		auto firstNonSpace = inString.find_first_not_of(" \t\r\n");
		if (firstNonSpace == -1)
			return "";
		else
			return inString.substr(firstNonSpace);
	}

	// remove trailing spaces
	static std::string rtrim(const std::string& inString) {
		return inString.substr(0, inString.find_last_not_of(" \t\r\n") + 1);
	}

	// remove both leading and trailing spaces
	static std::string trim(const std::string& inString) {
		return ltrim(rtrim(inString));
	}

	// change all letters of a string to capital letters
	static std::string upperCase(const std::string& inString);

	// change all letters of a string to lowercase letters
	static std::string lowerCase(const std::string& inString);

	// change first letter of each word to capital and all other letters to lowercase
	static std::string properCase(const std::string& inString);

	// converts a CLR system string into a "regular C++ string"
	static std::string convertString(System::String^ aString) {
		msclr::interop::marshal_context context;
		std::string standardString = context.marshal_as<std::string>(aString);
		return standardString;
	}

	// takes a string and splits it into a vector of strings using the delimiter 
	static std::vector<std::string> split(std::string str, std::string delim);

	// adds newline characters to a string to break it up at spaces into shorter lines
	// (will not break up words)
	static std::string wrapLongLine(const std::string& outString, int lineLength);

	// replaces all instances of "fromThis" in "original" to "toThat"
	static std::string replaceAll(const std::string& original,
		const std::string& fromThis, const std::string& toThat);

	// prints a number with given significant figures (rounding and adding zeros as needed)
	static std::string sigFig(double theValue, int figCount);

	static double roundToSigFigs(double value, int digits);

	static System::String^ sigFigs(double value, int digits) {
		//System::String^ result = gcnew System::String("");
		double sigValue = roundToSigFigs(value, digits);
		return gcnew System::String(sigValue.ToString());
	}

};
#include <sstream>
#include <iostream>
#include "StringPlus.h"

using namespace std;

string StringPlus::upperCase(const string& inString)
{
	string output = "";
	for (int i = 0; i < inString.length(); i++) {
		if ('a' <= inString[i] && inString[i] <= 'z')
			output += inString[i] + ('A' - 'a');
		else
			output += inString[i];
	}
	return output;
}

string StringPlus::lowerCase(const string& inString)
{
	string output = "";
	for (int i = 0; i < inString.length(); i++) {
		if ('A' <= inString[i] && inString[i] <= 'Z')
			output += inString[i] - ('A' - 'a');
		else
			output += inString[i];
	}
	return output;
}

string StringPlus::properCase(const string& inString)
{
	string output = "";
	bool firstLetter = true;
	int i = 0;
	while (i < inString.length()) {
		// advance to first letter, copying along the way
		while (i < inString.length()
			&& !('A' <= inString[i] && inString[i] <= 'Z')  // not a capital letter
			&& !('a' <= inString[i] && inString[i] <= 'z')) // not a lower-case letter
			output += inString[i++];   // note that using i++ here increments i AFTER using it

		// capitalize first letter of word if it is lower-case
		if ('a' <= inString[i] && inString[i] <= 'z')
			output += inString[i++] + ('A' - 'a');
		else
			output += inString[i++];

		// advance to next space, shifting any capital letters to lower case along the way
		while (i < inString.length() && inString[i] != ' ')
			if ('A' <= inString[i] && inString[i] <= 'Z')
				output += inString[i++] - ('A' - 'a');
			else
				output += inString[i++];

		// put space into output
		if (i < inString.length())
			output += inString[i++];
	}

	return output;
}

string StringPlus::wrapLongLine(const string& outString, int lineLength)
{
	int stringLength = outString.length();
	int startPosition = 0, currPosition = lineLength;
	string wrapped = "";

	while (currPosition < stringLength) {
		// work backward from lineLength position to find whitespace
		while (outString[currPosition] != ' ' && outString[currPosition] != '\n')
			currPosition--;

		// printout the substring
		wrapped += outString.substr(startPosition, currPosition - startPosition) + '\n';

		// reset for next line
		startPosition = currPosition + 1;
		currPosition = startPosition + lineLength;
	}

	// printout last part 
	wrapped += outString.substr(startPosition, currPosition - startPosition);

	return wrapped;
}

std::vector<std::string> StringPlus::split(std::string str, std::string delim)
{
	char* cstr = const_cast<char*>(str.c_str());
	char* current;
	std::vector<std::string> result;
	current = strtok(cstr, delim.c_str());
	while (current != NULL) {
		result.push_back(current);
		current = strtok(NULL, delim.c_str());
	}
	return result;
}

string StringPlus::replaceAll(const string& original, const string& fromThis, const string& toThat)
{
	string newString = original;  // make a copy
	size_t startPos = 0;

	while ((startPos = newString.find(fromThis, startPos)) != string::npos) // substring was found
		newString.replace(startPos, fromThis.length(), toThat);

	return newString;
}

string StringPlus::sigFig(double theValue, int figCount)
{
	std::stringstream theStream;
	theStream.precision(figCount);
	theStream << theValue;
	return theStream.str();
}

double StringPlus::roundToSigFigs(double value, int digits)
{
	if (value == 0.) // cannot take log of zero
		return 0.;
	else {
		int logOfGiven = log10(fabs(value));
		int decimalPlaces = digits - 1 - logOfGiven; // may be "negative" decimal places

		// use the following to round to decimalPlaces
		double adjust = pow(10., decimalPlaces);
		return round(value * adjust) / adjust;
	}

}


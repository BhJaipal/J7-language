#include "Trim.hpp"
#include <string>

std::string trim(std::string &str) {
	int start = 0;
	int end = str.length() - 1;

	// Find the first non-whitespace character from the start
	while (start < str.length() && isspace(str[start])) { start++; }

	// Find the last non-whitespace character from the end
	while (end >= 0 && isspace(str[end])) { end--; }

	// Return the trimmed substring
	return str.substr(start, end - start + 1);
}
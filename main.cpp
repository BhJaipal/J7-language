#include "utils/Errors.hpp"
#include <iostream>

int main() {
	std::cout << new J7::SyntaxError("Testing error");
	try {
		throw J7::Error("Error", "Unexpected error has occured", 6, "Ok");
	} catch (J7::Error e) { std::cerr << &e << '\n'; }
	return 0;
}
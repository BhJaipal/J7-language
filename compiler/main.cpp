#include "lexer/lexer.hpp"
#include "utils/Errors.hpp"
#include "utils/Find.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

int main(int argc, char **argv) {
	std::string src;
	{
		std::stringstream ss;
		std::fstream input(argv[1], std::ios::in);
		ss << input.rdbuf();
		src = ss.str();
	}
	std::vector<J7::Token> tokens = {{J7::TokenType::Int8, "7"},
									 {J7::TokenType::Identifier, "ok"}};

	std::cout << tokens;
	return 0;
}
#include "Lexer.hpp"
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
	std::cout << src << std::endl;
	std::vector<J7::Error *> err;
	std::vector<J7::Token> tokens = {{J7::TokenType::Int, "7"},
									 {J7::TokenType::Identifier, "ok"}};
	tokens = J7::tokenize(src, err);
	std::cout << tokens << std::endl;
	return 0;
}
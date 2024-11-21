#include "lexer.hpp"
#include "utils/Errors.hpp"
#include "utils/Find.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
std::string asmArgsToRegisters(std::string arg) {
#if defined(__x86_64__) && defined __LP64__
	if (arg == "arg0") return "rdi";
	else if (arg == "arg1") return "rsi";
	else if (arg == "arg2") return "rdx";
	else if (arg == "arg3") return "rcx";
	else if (arg == "return") return "rax";
	else return "rax";
#elif defined(__aarch64__)
	if (arg == "arg0") return "x0";
	else if (arg == "arg1") return "x1";
	else if (arg == "arg2") return "x2";
	else if (arg == "arg3") return "x3";
	else if (arg == "return") return "x0";
	else return "x0";
#elif defined(__arm__)
	if (arg == "arg0") return "r0";
	else if (arg == "arg1") return "r1";
	else if (arg == "arg2") return "r2";
	else if (arg == "arg3") return "r3";
	else if (arg == "return") return "r0";
	else return "r0";
#elif !defined __x86_64__
	if (arg == "arg0") return "ebx";
	else if (arg == "arg1") return "ecx";
	else if (arg == "arg2") return "edx";
	else if (arg == "arg3") return "esi";
	else if (arg == "return") return "eax";
	else return "eax";
#endif
}

int main(int argc, char **argv) {
	std::string src;
	{
		std::stringstream ss;
		std::fstream input(argv[1], std::ios::in);
		ss << input.rdbuf();
		src = ss.str();
	}
	J7::Lexer lexer(src);
	std::vector<J7::Token> tokens = lexer.tokenize();

	std::cout << tokens;
	return 0;
}
#include "lexer/lexer.hpp"
#include "utils/Errors.hpp"
#include "utils/Find.hpp"
#include <fstream>
#include <iostream>
#include <llvm-16/llvm/IR/Module.h>
#include <sstream>
#include <vector>
std::string asmArgsToRegisters(std::string arg) {
#if defined(__x86_64__) && defined __LP64__
	if (arg == "arg0") return "rdi";
	if (arg == "arg1") return "rsi";
	if (arg == "arg2") return "rdx";
	if (arg == "arg3") return "rcx";
	if (arg == "return") return "rax";
#elif defined(__aarch64__)
	if (arg == "arg0") return "x0";
	if (arg == "arg1") return "x1";
	if (arg == "arg2") return "x2";
	if (arg == "arg3") return "x3";
	if (arg == "return") return "x0";
#elif defined(__arm__)
	if (arg == "arg0") return "r0";
	if (arg == "arg1") return "r1";
	if (arg == "arg2") return "r2";
	if (arg == "arg3") return "r3";
	if (arg == "return") return "r0";
#elif !defined __x86_64__
	if (arg == "arg0") return "ebx";
	if (arg == "arg1") return "ecx";
	if (arg == "arg2") return "edx";
	if (arg == "arg3") return "esi";
	if (arg == "return") return "eax";
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
	std::vector<J7::Token> tokens = J7::lex(src);
	// std::vector<J7::Token> tokens = {{J7::TokenType::Int8, "7"},
	// 								 {J7::TokenType::Identifier, "ok"}};

	std::cout << tokens;
	return 0;
}
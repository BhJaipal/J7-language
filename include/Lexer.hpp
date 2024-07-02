#pragma once
#include "modules/my_hashmap.hpp"
#include <string>
#include <vector>

namespace J7 {
enum class TokenType {
	Int,
	Float,
	Bool,
	String,
	Var,
	Const,
	Function,
	Return,
	If,
	Else,
	Elif,
	While,
	For,
	Equals,
	UrinaryOperator,
	BinaryOperator,
	OpenParen,
	CloseParen,
	OpenSquare,
	CloseSquare,
	OpenCurly,
	CloseCurly,
	Identifier,
	Comment,
	EOLine,
	EOFile,
	Invalid
};
struct Token {
	TokenType type;
	std::string literal;
};
struct TokenIdentifier {
	std::vector<std::string> identifiers;
	static TokenIdentifier
		tokenIdentifier[static_cast<int>(TokenType::Invalid)];
};
void processEscChars(std::string &str);
void unprocessEscChars(std::string &str);

std::string extractNextIdentifier(std::string &line, char seperator);

std::ostream &operator<<(std::ostream &os, const Token &token);
std::ostream &operator<<(std::ostream &os, const std::vector<Token> &token);
} // namespace J7
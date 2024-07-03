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

std::vector<Token> &tokenize(const std::string &source);

std::ostream &operator<<(std::ostream &os, const Token &token);
std::ostream &operator<<(std::ostream &os, const std::vector<Token> &token);
} // namespace J7
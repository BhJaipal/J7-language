#pragma once
#include "modules/my_hashmap.hpp"
#include <string>
#include <vector>

namespace J7 {
enum class TokenType {
	Int32,
	Int64,
	Int16,
	Int8,
	UInt8,
	UInt16,
	UInt32,
	UInt64,
	Float32,
	Float64,
	LongFloat64,
	Bool,
	String,
	Array,
	Set,
	Tuple,
	Map,
	VarValue,
	ConstValue,
	Function,
	Return,
	If,
	Else,
	Elif,
	While,
	For,
	In,
	Break,
	Continue,
	Equals,
	UrinaryOperator,
	BinaryOperator,
	AssignmentOperator,
	TypeAssignmentOperator,
	MemberAccess,
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

std::vector<Token> lex(std::string src);

std::ostream &operator<<(std::ostream &os, const Token &token);
std::ostream &operator<<(std::ostream &os, const std::vector<Token> &token);
} // namespace J7
#ifndef J7_LEXER
#define J7_LEXER

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
	NextIdentifier,
	Comment,
	EOLine,
	EOFile,
	Invalid
};
struct Token {
	TokenType type;
	std::string literal;
};

static std::string keywords[] = {
	"var",	"val",	 "fun", "ret", "if",	"elif",
	"else", "while", "for", "in",  "break", "continue",
};

static std::string types[] = {
	"int8",	  "int16",	"int32",   "int64",	  "uint8",	   "uint16",
	"uint32", "uint64", "float32", "float64", "longfloat", "bool",
	"string", "array",	"tuple",   "set",	  "map",
};
static std::vector<char> symbols{
	'(', ')', '{', '}', '[', ']', '.', ',', ';',  ':',	'+',  '-',	'=',  '<',
	'>', '%', '&', '|', '^', '~', '!', '?', '\"', '\'', '\\', '\t', '\n', '\r',
};
static std::vector<char> singleSymbol{
	'(', ')', '{', '}', '[', ']', '.', ',', ';', ':', ';', '"', '\'',
};

std::vector<Token> lex(std::string src);

std::ostream &operator<<(std::ostream &os, const Token &token);
std::ostream &operator<<(std::ostream &os, const std::vector<Token> &token);
} // namespace J7
#endif // J7_LEXER

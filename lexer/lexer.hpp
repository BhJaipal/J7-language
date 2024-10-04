#ifndef J7_LEXER
#define J7_LEXER

#include "modules/my_hashmap.hpp"
#include <map>
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

static std::map<TokenType, std::string> keySwitch = {
	{TokenType::Int32, "Int32"},
	{TokenType::Int64, "Int64"},
	{TokenType::Int16, "Int16"},
	{TokenType::Int8, "Int8"},
	{TokenType::UInt8, "UInt8"},
	{TokenType::UInt16, "UInt16"},
	{TokenType::UInt32, "UInt32"},
	{TokenType::UInt64, "UInt64"},
	{TokenType::Float32, "Float32"},
	{TokenType::Float64, "Float64"},
	{TokenType::LongFloat64, "LongFloat64"},
	{TokenType::Bool, "Bool"},
	{TokenType::String, "String"},
	{TokenType::Array, "Array"},
	{TokenType::Set, "Set"},
	{TokenType::Tuple, "Tuple"},
	{TokenType::Map, "Map"},
	{TokenType::VarValue, "VarValue"},
	{TokenType::ConstValue, "ConstValue"},
	{TokenType::Function, "Function"},
	{TokenType::Return, "Return"},
	{TokenType::If, "If"},
	{TokenType::Else, "Else"},
	{TokenType::Elif, "Elif"},
	{TokenType::While, "While"},
	{TokenType::For, "For"},
	{TokenType::In, "In"},
	{TokenType::Break, "Break"},
	{TokenType::Continue, "Continue"},
	{TokenType::Equals, "Equals"},
	{TokenType::UrinaryOperator, "UrinaryOperator"},
	{TokenType::BinaryOperator, "BinaryOperator"},
	{TokenType::AssignmentOperator, "AssignmentOperator"},
	{TokenType::TypeAssignmentOperator, "TypeAssignmentOperator"},
	{TokenType::MemberAccess, "MemberAccess"},
	{TokenType::OpenParen, "OpenParen"},
	{TokenType::CloseParen, "CloseParen"},
	{TokenType::OpenSquare, "OpenSquare"},
	{TokenType::CloseSquare, "CloseSquare"},
	{TokenType::OpenCurly, "OpenCurly"},
	{TokenType::CloseCurly, "CloseCurly"},
	{TokenType::Identifier, "Identifier"},
	{TokenType::NextIdentifier, "NextIdentifier"},
	{TokenType::Comment, "Comment"},
	{TokenType::EOLine, "EOLine"},
	{TokenType::EOFile, "EOFile"},
	{TokenType::Invalid, "Invalid"},
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

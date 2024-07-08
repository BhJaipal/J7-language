#include "Lexer.hpp"
#include "utils/Errors.hpp"
#include "utils/Find.hpp"
#include <map>
#include <string>

namespace J7 {
std::vector<std::string> J7Keywords{"ret",	 "for",	   "if",  "elif",
									"else",	 "import", "fun", "var",
									"const", "while",  "from"};
std::vector<std::string> operators = {
	"=",  "+",	"-", "*", "/", "%", ">", "<", ">=", "<=", "==", "!=",
	"&&", "||", "=", ":", ".", ",", ";", "(", ")",	"[",  "]"};
std::vector<char> symbols{';', '<', '>', ':', ',', '=', '+', '-', '*',
						  '/', '%', '(', ')', '[', ']', '{', '}'};
std::vector<char> singleSym{'[', '{', '(', ')', '}', ']', '=', ';'};
std::vector<char> allSymbols{'~', '`', '<',	 '>', '\"', '\'', ':', ',',
							 ';', '.', '\\', '/', '!',	'@',  '#', '$',
							 '%', '^', '&',	 '*', '(',	')',  '-', '+',
							 '=', '[', ']',	 '{', '}',	'|',  '?', ' '};
std::vector<std::string> J7Types{"Int", "Float", "String"};
std::vector<std::string> J7TypesWithTemplate{"Array", "Set", "Map"};
std::map<std::string, TokenType> J7KeywordMap{
	{"var", TokenType::Var},	  {"const", TokenType ::Const},
	{"fun", TokenType::Function}, {"ret", TokenType::Return},
	{"if", TokenType::If},		  {"elif", TokenType::Elif},
	{"else", TokenType::Else},	  {"while", TokenType::While},
	{"for", TokenType::For},	  {"import", TokenType::Import},
	{"from", TokenType::From},	  {"true", TokenType::Bool},
	{"false", TokenType::Bool}};

std::map<TokenType, std::string> J7TokenLabel{
	{TokenType::Int, "int"},
	{TokenType::Float, "float"},
	{TokenType::String, "string"},
	{TokenType::Bool, "bool"},
	{TokenType::Array, "array"},
	{TokenType::Set, "set"},
	{TokenType::Map, "map"},
	{TokenType::Var, "keyword"},
	{TokenType::Const, "keyword"},
	{TokenType::Function, "keyword"},
	{TokenType::Return, "keyword"},
	{TokenType::BinaryOperator, "binary operator"},
	{TokenType::UrinaryOperator, "urinary operator"},
	{TokenType::If, "keyword"},
	{TokenType::Elif, "keyword"},
	{TokenType::Else, "keyword"},
	{TokenType::While, "keyword"},
	{TokenType::Equals, "equals"},
	{TokenType::For, "keyword"},
	{TokenType::Import, "keyword"},
	{TokenType::From, "keyword"},
	{TokenType::EOLine, "EOLine"},
	{TokenType::Comment, "comment"},
	{TokenType::Identifier, "Identifier"},
};

bool isWhiteSpace(char c) {
	switch (c) {
	case '\t': return true;
	case '\n': return true;
	case ' ': return true;
	case '\b': return true;
	case '\r': return true;
	default: return false;
	}
}

std::vector<Token> tokenize(const std::string &source,
							std::vector<Error *> &errors) {
	std::vector<Token> tokens;
	std::string identifier;
	int lineNo = 1;
	bool isComment = false, isString = false, isMultiline = false;
	char quoteType = ' ';
	int nextLine = Find(source, '\n');
	std::string line =
		source.substr(0, nextLine != -1 ? nextLine : source.size());
	std::string remaining = nextLine != -1 ? source.substr(nextLine + 1) : "";
	int onlyOnce = remaining.size() == 0;
	while (onlyOnce || line.size()) {
		for (int i = 0; i < line.size(); i++) {
			if (isString) {
				if (line[i] == quoteType && (i > 0 && line[i - 1] != '\\')) {
					isString = false;
					identifier += line[i];
					tokens.push_back({TokenType::String, identifier});
					identifier = "";
					quoteType = ' ';
				} else {
					identifier += line[i];
				}
				continue;
			} else if (isComment) {
				if (!isMultiline) {
				} else if (line[i] != '/') {
				} else if (i > 0 && line[i - 1] == '*') {
					isMultiline = false;
					isComment = false;
				}
				continue;
			}
			if (line[i] == '"' || line[i] == '\'') {
				identifier = line[i];
				quoteType = line[i];
				isString = true;
				continue;
			}
			if (identifier == "") { identifier += line[i]; }
			if (identifier == " ") { identifier = line[i]; }
		MatchIdentifier:
			if (std::isalpha(identifier[0]) || identifier[0] == '_') {
				if (isWhiteSpace(line[i]) || Find(symbols, line[i]) != -1) {
					for (int i = 1; i < identifier.size(); i++) {
						if (!(std::isalnum(identifier[i]) ||
							  identifier[i] == '_')) {
							tokens.push_back({TokenType::Invalid, identifier});
							errors.push_back(new J7::SyntaxError(
								"Invalid identifier", lineNo, line));
							identifier = "";
							break;
						}
					}
					int i = J7KeywordMap.find(identifier) != J7KeywordMap.end();
					if (i)
						tokens.push_back(
							{J7KeywordMap[identifier], identifier});
					else tokens.push_back({TokenType::Identifier, identifier});
					identifier = "";
					if (Find(symbols, line[i]) != -1) { identifier += line[i]; }
					continue;
				}
				if (identifier.size() == 1 && identifier[0] == line[i])
					continue;
				identifier += line[i];
			}
			if (std::isdigit(identifier[0]) || identifier[0] == '.') {
				int hasDot = 0;
				if (identifier[0] == '.') {
					identifier = "0" + identifier;
					hasDot = 1;
				}
				for (int i = 1; i < identifier.size(); i++) {
					if (!(std::isdigit(identifier[i]) ||
						  identifier[i] == '.')) {
						tokens.push_back({TokenType::Invalid, identifier});
						errors.push_back(new J7::SyntaxError("Invalid number",
															 lineNo, line));
						identifier = "";
						break;
					}
					if (identifier[i] == '.') hasDot = 1;
				}
				if (hasDot) tokens.push_back({TokenType::Float, identifier});
				else tokens.push_back({TokenType::Int, identifier});
				identifier = "";
			}
			if (Find(symbols, line[i]) != -1) {
				if (Find(singleSym, line[i]) != -1) {
					TokenType symType;
					switch (line[i]) {
					case '[': symType = TokenType::OpenSquare; break;
					case ']': symType = TokenType::CloseSquare; break;
					case '{': symType = TokenType::OpenCurly; break;
					case '}': symType = TokenType::CloseCurly; break;
					case '(': symType = TokenType::OpenParen; break;
					case ')': symType = TokenType::CloseParen; break;
					case '=': symType = TokenType::Equals; break;
					case ';': symType = TokenType::EOLine; break;
					default: break;
					}
					tokens.push_back({symType, identifier});
					identifier = "";
					continue;
				} else {
					identifier += line[i];
				}
			}
			if (Find(symbols, line[i]) != -1) {
				identifier += line[i];
			} else if (!isalnum(line[i]) && line[i] != '_' &&
					   !isdigit(line[i]) && !isspace(line[i]) &&
					   Find(allSymbols, line[i]) != -1) {
				errors.push_back(new SyntaxError(
					"Unknown symbol '" + std::string(1, (char)line[i]) + "'",
					(unsigned int)lineNo, line));
				identifier = "";
			}
			if (Find(operators, identifier) != -1 && isalnum(line[i]) &&
				isspace(line[i])) {
				tokens.push_back({TokenType::BinaryOperator, identifier});
				identifier = "";
			}
		}
	ToNext:
		if (isComment && !isMultiline) {
			isComment = false;
			tokens.push_back({TokenType::Comment, identifier});
			identifier = "";
		}
		if (onlyOnce) break;
		int next = Find(remaining, '\n');
		if (next == -1) {
			line = remaining;
			remaining = "";
		} else {
			line = remaining.substr(0, next);
			remaining = remaining.substr(next + 1);
		}
		lineNo++;
	}
RetTokens:
	return tokens;
}

std::ostream &operator<<(std::ostream &os, const Token &token) {
	std::string value = token.type == TokenType::EOLine ? ";" : token.literal;
	os << "Token {type: " << J7TokenLabel[token.type] << ", value: " << value
	   << "}\n";
	return os;
}

std::ostream &operator<<(std::ostream &os, const std::vector<Token> &tokens) {
	for (auto &token : tokens) os << token;
	return os;
}

} // namespace J7
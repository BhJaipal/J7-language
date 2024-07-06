#include "Lexer.hpp"
#include "utils/Errors.hpp"
#include <map>
#include <string>

namespace J7 {
std::vector<std::string> J7Keywords{"ret",	 "for",	   "if",  "elif",
									"else",	 "import", "fun", "var",
									"const", "while",  "from"};
std::vector<std::string> operators = {
	"=",  "+",	"-",  "*", "/", "%", ">", "<", ">=", "<=", "==", "!=",
	"&&", "||", ":=", ":", ".", ",", ";", "(", ")",	 "[",  "]",
};
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
	{TokenType::Var, "var"},
	{TokenType::Const, "const"},
	{TokenType::Function, "fun"},
	{TokenType::Return, "ret"},
	{TokenType::If, "if"},
	{TokenType::Elif, "elif"},
	{TokenType::Else, "else"},
	{TokenType::While, "while"},
	{TokenType::For, "for"},
	{TokenType::Import, "import"},
	{TokenType::From, "from"},
	{TokenType::EOLine, "EOLine"},
	{TokenType::Comment, "comment"},
	{TokenType::Identifier, "Identifier"},
};

int isWhiteSpace(char c) {
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
	std::string line = source.substr(0, source.find('\n'));
	std::string remaining = source.substr(source.find('\n'));
	while (remaining.size()) {
		for (int i = 0; i < line.size(); i++) {
			if (isString) {
				if (line[i] == quoteType && (i > 0 && line[i - 1] != '\\')) {
					isString = false;
					identifier += line[i];
					tokens.push_back({TokenType::String, identifier});
					identifier = "";
					quoteType = ' ';
					goto ToNext;
				} else {
					identifier += line[i];
					goto ToNext;
				}
			} else if (isComment) {
				if (!isMultiline) {
					identifier += line[i];
					goto ToNext;
				} else if (line[i] != '/') {
					identifier += line[i];
					goto ToNext;
				} else if (i > 0 && line[i - 1] == '*') {
					identifier += line[i];
					isMultiline = false;
					isComment = false;
					tokens.push_back({TokenType::Comment, identifier});
					identifier = "";
					goto ToNext;
				}
			}
			if (line[i] == '"' || line[i] == '\'') {
				identifier = line[i];
				quoteType = line[i];
				isString = true;
			}
			if (isWhiteSpace(line[i])) { goto MatchIdentifier; }
		MatchIdentifier:
			if (std::isalpha(identifier[0]) || identifier[0] == '_') {
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
				if (J7KeywordMap.find(identifier) != J7KeywordMap.end())
					tokens.push_back({J7KeywordMap[identifier], identifier});
				else tokens.push_back({TokenType::Identifier, identifier});
				identifier = "";
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
		}
	ToNext:
		if (isComment && !isMultiline) {
			isComment = false;
			tokens.push_back({TokenType::Comment, identifier});
			identifier = "";
		}
		line = remaining.substr(0, remaining.find('\n'));
		remaining = remaining.substr(remaining.find('\n'));
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
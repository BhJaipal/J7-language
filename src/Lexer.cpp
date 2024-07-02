#include "Lexer.hpp"

namespace J7 {
TokenIdentifier
	TokenIdentifier::tokenIdentifier[static_cast<int>(TokenType::Invalid)] = {
		{{}},
		{{}},
		{{}},
		{{}},
		{{"var"}},
		{{"const"}},
		{{"fun"}},
		{{"return"}},
		{{"if"}},
		{{"else"}},
		{{"elif"}},
		{{"while"}},
		{{"for"}},
		{{"="}},
		{{"++", "--", "+", "-", "!"}},
		{{"+", "-", "*", "/", "%", "^", "&", "|", "<<", ">>", "||", "&&",
		  "==", "!=", "<", ">", "<=", ">="}},
		{{"("}},
		{{")"}},
		{{"["}},
		{{"]"}},
		{{"{"}},
		{{"}"}},
		{{}},
		{{"//", "/*", "*/"}},
		{{";"}}};

void processEscChars(std::string &str) {
	for (int i = 0; i < str.size(); i++) {
		char charAtI = str[i];

		if (charAtI == '\\' && i < str.size() - 1) {
			char nextChar = str[i + 1];
			char replacementChar = ' ';
			switch (nextChar) {
			case 'n': replacementChar = '\n'; break;
			case 't': replacementChar = '\t'; break;
			case 'r': replacementChar = '\r'; break;
			case 'b': replacementChar = '\b'; break;
			case 'f': replacementChar = '\f'; break;
			}
			if (replacementChar != ' ') {
				str[i] = replacementChar;
				str.erase(str.begin() + i + 1);
			}
		}
	}
}

void unprocessEscChars(std::string &str) {
	for (int i = 0; i < str.size(); i++) {
		char charAtI = str[i];
		std::string insertionStr;
		switch (charAtI) {
		case '\n': insertionStr = "\\n"; break;
		case '\t': insertionStr = "\\t"; break;
		case '\r': insertionStr = "\\r"; break;
		case '\b': insertionStr = "\\b"; break;
		case '\f': insertionStr = "\\f"; break;
		}
		if (!insertionStr.empty()) {
			str.erase(str.begin() + i);
			str.insert(i, insertionStr);
			i++;
		}
	}
}

std::ostream &operator<<(std::ostream &os, const Token &token) {
	std::string value =
		token.type == TokenType::EOLine ? "EOLine;" : token.literal;
	unprocessEscChars(value);
	os << "Token {type: " << static_cast<int>(token.type)
	   << ", value: " << value << "}\n";
	return os;
}

std::ostream &operator<<(std::ostream &os, const std::vector<Token> &tokens) {
	for (auto &token : tokens) os << token;
	return os;
}

} // namespace J7
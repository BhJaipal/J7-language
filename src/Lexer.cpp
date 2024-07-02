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

std::string extractNextIdentifier(std::string &line, char seperator) {
	const char skippableChars[] = {' ', '\t', '\n', '\0'};
	const char nonChainableChars[] = {'[', '(', '{', '}', ']', ')'};
	enum class IdentifierType { Alpha, Ints, Floats, Symbolic, Unknown };
	auto isSkippable = [&skippableChars](char c) -> bool {
		for (char skippable : skippableChars) {
			if (c == skippable) return true;
		}
		return false;
	};
	auto isUnchainableChar = [&nonChainableChars](char c) -> bool {
		for (char nonChainableChar : nonChainableChars) {
			if (c == nonChainableChar) return true;
		}
		return false;
	};
	auto determineIdentifierType = [](char c) -> IdentifierType {
		return (c == '_' || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
				   ? IdentifierType::Alpha
				   : (c >= '0' && c <= '9'
						  ? IdentifierType::Ints
						  : ((c == '.' || c == '-') || (c >= '0' && c <= '9')
								 ? IdentifierType::Floats
								 : IdentifierType::Symbolic));
	};
	IdentifierType idType = IdentifierType::Unknown;
	std::string nextIdentifier;
	std::size_t nextIdentifierEnd = 0;
	bool isString = false;
	bool isComment = false;
	bool isMultilineComment = false;
	char lastCharacter = ' ';
	for (auto charInLine : line) {
		if (isComment) {
			nextIdentifierEnd++;
			if (charInLine == '\n') break;
			continue;
		}
		if (isUnchainableChar(charInLine)) {
			if (nextIdentifierEnd == 0) {
				nextIdentifier += charInLine;
				nextIdentifierEnd++;
			}
			break;
		}
		if (!isSkippable(charInLine) || isString) {
			if (!isComment) {
				if (charInLine == '\"' || charInLine == '\'') {
					if (isString) {
						nextIdentifierEnd++;
						break;
					}
					isString = true;
				} else if (charInLine == '/' && lastCharacter == '/' &&
						   !isString) {
					isComment = true;
					isMultilineComment = false;
					if (nextIdentifier.size() > 0) break;
					else nextIdentifierEnd++;
					continue;
				}
				if (isString) {
					nextIdentifierEnd++;
					nextIdentifier += charInLine;
					continue;
				}
				if (charInLine == '*' || lastCharacter == '/' && !isString) {
					isMultilineComment = true;
					isComment = true;
					if (nextIdentifier.size() > 0) break;
					else nextIdentifierEnd++;
					continue;
				}
			}
			IdentifierType currIdType = determineIdentifierType(charInLine);
		} else if (nextIdentifier.size() > 0) break;
		nextIdentifierEnd++;
		lastCharacter = charInLine;
	}
	line = line.substr(nextIdentifierEnd);
	return nextIdentifier;
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
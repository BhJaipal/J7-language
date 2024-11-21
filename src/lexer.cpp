#include "lexer.hpp"
#include "utils/Find.hpp"
#include <map>
#include <string>

namespace J7 {
Token strNum_to_token(const std::string &identifier, const int is_float) {
	if (is_float) {
		if (std::stold(identifier) > 1.7E-308 ||
			std::stold(identifier) < 1.7E-308)
			return {TokenType::LongFloat64, identifier};
		else if (std::stod(identifier) > 3.4E+38 ||
				 std::stod(identifier) < 1.2E-38)
			return {TokenType::Float64, identifier};
		else return {TokenType::Float32};
	} else {
		if (std::stoul(identifier) <= 255)
			return {TokenType::UInt8, identifier};
		else if (std::stoull(identifier) > 255)
			return {TokenType::UInt16, identifier};
		else if (std::stoul(identifier) > 65535)
			return {TokenType::UInt32, identifier};
		else if (std::stoull(identifier) > 4294967295)
			return {TokenType::UInt64, identifier};
		else if (std::stoll(identifier) > 2147483647 &&
				 std::stoll(identifier) < -2147483648)
			return {TokenType::Int64, identifier};
		else if (std::stoi(identifier) > 32767 &&
				 std::stoi(identifier) < -32768)
			return {TokenType::Int32, identifier};
		else if (std::stoi(identifier) > 127 && std::stoi(identifier) < -128)
			return {TokenType::Int16, identifier};
		else return {TokenType::Int8, identifier};
	}
}

Token Lexer::strKey_identifier_to_token(const std::string &identifier) const {
	for (const auto & keyword : keywords) {
		if (identifier == keyword) {
			if (identifier == "var") return {TokenType::VarValue, identifier};
			if (identifier == "val") return {TokenType::ConstValue, identifier};
			if (identifier == "fun") return {TokenType::Function, identifier};
			if (identifier == "ret") return {TokenType::Return, identifier};
			if (identifier == "if") return {TokenType::If, identifier};
			if (identifier == "elif") return {TokenType::Elif, identifier};
			if (identifier == "else") return {TokenType::Else, identifier};
			if (identifier == "while") return {TokenType::While, identifier};
			if (identifier == "for") return {TokenType::For, identifier};
			if (identifier == "in") return {TokenType::In, identifier};
			if (identifier == "break") return {TokenType::Break, identifier};
			if (identifier == "continue")
				return {TokenType::Continue, identifier};
		}
	}
	for (const auto & type : types) {
		if (identifier == type) {
			if (identifier == "int8") return {TokenType::Int8, identifier};
			if (identifier == "int16") return {TokenType::Int16, identifier};
			if (identifier == "int32") return {TokenType::Int32, identifier};
			if (identifier == "int64") return {TokenType::Int64, identifier};
			if (identifier == "uint8") return {TokenType::UInt8, identifier};
			if (identifier == "uint16") return {TokenType::UInt16, identifier};
			if (identifier == "uint32") return {TokenType::UInt32, identifier};
			if (identifier == "uint64") return {TokenType::UInt64, identifier};
			if (identifier == "float32")
				return {TokenType::Float32, identifier};
			if (identifier == "float64")
				return {TokenType::Float64, identifier};
			if (identifier == "lfloat64")
				return {TokenType::LongFloat64, identifier};
			if (identifier == "bool") return {TokenType::Bool, identifier};
			if (identifier == "string") return {TokenType::String, identifier};
			if (identifier == "array") return {TokenType::Array, identifier};
			if (identifier == "tuple") return {TokenType::Tuple, identifier};
			if (identifier == "map") return {TokenType::Map, identifier};
			if (identifier == "set") return {TokenType::Set, identifier};
		}
	}
	return {TokenType::Identifier, identifier};
}

std::vector<Token> Lexer::tokenize() const {
	std::vector<Token> tokens;
	std::string identifier;
	char stringStart = 0;
	int is_digit = 0, is_float = 0, alpha = 0, is_comment = 0, is_string = 0, is_multiline = 0;
	for (std::size_t i =0; i< src.size(); i++) {
		char ch = src[i];
		if (is_comment) {
			if (src.size() -1 == i) {
				identifier+= ch;
				tokens.push_back({TokenType::Comment, identifier});
				return tokens;
			}
			if (is_multiline && ch == '*' && i+1 < src.size() && src[i+1] == '/') {
				is_comment = 0;
				is_multiline = 0;
				i++;
			} else if (!is_multiline && ch == '\n') {
				tokens.push_back({TokenType::Comment, identifier});
				identifier.clear();
			} else identifier += ch;
			continue;
		}
		if (is_string) {
			if (stringStart == ch && src[i-1] != '\\') {
				is_string = 0;
				tokens.push_back({TokenType::String, identifier});
				stringStart = 0;
			} else {
				identifier += ch;
			}
			continue;
		}
		if (ch == '"' || ch == '\'') {
			stringStart = ch;
			is_string = 1;
		}
		if (ch == '(') { tokens.push_back({TokenType::OpenParen, "("}); continue; }
		if (ch == ')') { tokens.push_back({TokenType::CloseParen, ")"}); continue; }
		if (ch == '[') { tokens.push_back({TokenType::OpenSquare, "["}); continue; }
		if (ch == ']') { tokens.push_back({TokenType::CloseSquare, "]"}); continue; }
		if (ch == '{') { tokens.push_back({TokenType::OpenCurly, "{"}); continue; }
		if (ch == '}') { tokens.push_back({TokenType::CloseCurly, "}"}); continue; }
		if (ch == ',') { tokens.push_back({TokenType::Comma, ","}); continue; }
		if (ch == '.') { tokens.push_back({TokenType::CloseParen, "."}); continue; }
		if (ch == ';') { tokens.push_back({TokenType::EOLine, ";"}); continue; }
		if (ch == '/' && src.size() -1 > i) {
			if (src[i+1] == '*') {
				is_comment = true;
				i++;
				is_multiline = true;
			}
			if (src[i+1] == '/') {
				is_comment = true;
				i++;
				is_multiline = false;
			}
		}
		if (identifier.empty()) {
			if (isspace(src[i])) {
				continue;
			}
			identifier += src[i];
			if (isdigit(src[i])) is_digit = 1;
			else if (ch == '.') { is_digit = 1, is_float = 1; }
			else if (isalpha(ch)) alpha = 1;
			continue;
		}
		if (alpha) {
			if (isspace(ch) || !(isalpha(ch) || isdigit(ch) || ch == '_')) {
				if (const int keywordIndex = Find(keywords, identifier); keywordIndex == -1) {
					tokens.push_back({TokenType::Identifier, identifier});
					identifier.clear();
				} else {
					TokenType tokenType;
					switch (keywordIndex) {
						case 0:
							tokenType = TokenType::VarValue;
						break;
						case 1:
							tokenType = TokenType::ConstValue;
						break;
						case 2:
							tokenType = TokenType::Function;
						break;
						case 3:
							tokenType = TokenType::Return;
						break;
						case 4:
							tokenType = TokenType::If;
						break;
						case 5:
							tokenType = TokenType::Elif;
						break;
						case 6:
							tokenType = TokenType::Else;
						break;
						case 7:
							tokenType = TokenType::While;
						break;
						case 8:
							tokenType = TokenType::For;
						break;
						case 9:
							tokenType = TokenType::In;
						break;
						case 10:
							tokenType = TokenType::Break;
						break;
						default:
							tokenType = TokenType::Continue;
						break;
					}
					tokens.push_back({tokenType, identifier});
					identifier.clear();
				}
			}
			else
				identifier += ch;
		}
		else if (is_digit) {
			while (i < src.size() && (isdigit(ch) || ch == '_' || ch == 'e' || ch == 'E')) {
				ch = src[i];
				if (isdigit(ch) || ch == '.' || ch == '_' || ((ch == 'e' || ch == 'E') && isdigit(*identifier.end()))) {
					if (ch == '.' && identifier.empty()) {
						identifier += std::string("0") + ch;
						is_float = 1;
					}
					if (ch == 'e' || ch == 'E') is_float = 1;
					else identifier += ch;
				}
				i++;
			}
			tokens.push_back({ is_float ? TokenType::Float64 : TokenType::Int64, identifier});
		} else {
			if (isspace(ch)) {
				if (Find(binarySym, identifier) != -1) {
					tokens.push_back({TokenType::BinaryOperator, identifier});
				} else if (Find(unarySym, identifier) != -1) {
					tokens.push_back({TokenType::UrinaryOperator, identifier});
				}
				identifier.clear();
			} else if (isdigit(ch) || ch == '.' || ch == '_' || isalpha(ch)) {
				if (Find(binarySym, identifier) != -1) {
					tokens.push_back({TokenType::BinaryOperator, identifier});
				} else if (Find(unarySym, identifier) != -1) {
					tokens.push_back({TokenType::UrinaryOperator, identifier});
				}
				identifier.clear();
			} else {
				identifier += ch;
			}
		}
	}
	return tokens;
}
std::vector<std::string> Lexer::split(const std::string &source) {
	std::vector<std::string> tokens;
	std::string word;
	for (const char ch: source) {
		if (ch != ' ') word += ch;
		else if (!word.empty()) {
			tokens.push_back(word);
			word.clear();
		}
	}
	if (!word.empty()) tokens.push_back(word);
	return tokens;
}

std::ostream &operator<<(std::ostream &os, const Token &token) {
	os << keySwitch[token.type] << " " << token.literal;
	return os;
}
std::ostream &operator<<(std::ostream &os, const std::vector<Token> &tokens) {
	for (const auto & token : tokens) {
		os << (J7::Token)token << "\n";
	}
	return os;
}
} // namespace J7
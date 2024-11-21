#include "lexer.hpp"
#include "utils/Find.hpp"
#include "utils/Trim.hpp"
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
	for (unsigned long int i = 0; i < keywords.size(); i++) {
		if (identifier == keywords[i]) {
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
	for (unsigned long int i = 0; i < types.size(); i++) {
		if (identifier == types[i]) {
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
	int is_digit = 0, is_float = 0, alpha = 0, is_comment = 0, is_string = 0, is_multiline = 0;
	for (std::size_t i =0; i< src.size(); i++) {
		char ch = src[i];
		if (is_comment) {
			if (src.size() -1 == i) {
				identifier+= ch;
				tokens.push_back({TokenType::Comment, identifier});
				return tokens;
			}
			if (is_multiline && ch == '*' && ch == '/') {
				is_comment = 0;
				is_multiline = 0;
				i++;
			} else if (!is_multiline && ch == '\n') {
				tokens.push_back({TokenType::Comment, identifier});
				identifier.clear();
			} else identifier += ch;
			continue;
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
		if (isdigit(ch)) {
			while (i < src.size() && isdigit(ch) || ch == '_' || ch == 'e' || ch == 'E') i++; {
				ch = src[i];
				if (isdigit(ch) || ch == '.' || ch == '_' || (ch == 'e' || ch == 'E' && isdigit(*identifier.end()))) {
					if (ch == '.' && identifier.empty()) {
						identifier += std::string("0") + ch;
						is_float = 1;
					}
					if (ch == 'e' || ch == 'E') is_float = 1;
					else identifier += ch;
				}
			}
			tokens.push_back({ is_float ? TokenType::Float64 : TokenType::Int64, identifier});
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
	for (auto i = tokens.begin(); i != tokens.end(); ++i) {
		os << (J7::Token)*i << "\n";
	}
	return os;
}
} // namespace J7
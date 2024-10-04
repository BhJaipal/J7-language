#include "lexer.hpp"
#include "utils/Find.hpp"
#include "utils/Trim.hpp"
#include <map>
#include <string>

namespace J7 {
Token strNum_to_token(std::string identifier, int is_float) {
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

Token Lexer::strKeyIden_to_token(std::string identifier) {
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
			continue;
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
			if (identifier == "longfloat64")
				return {TokenType::LongFloat64, identifier};
			if (identifier == "bool") return {TokenType::Bool, identifier};
			if (identifier == "string") return {TokenType::String, identifier};
			if (identifier == "array") return {TokenType::Array, identifier};
			if (identifier == "tuple") return {TokenType::Tuple, identifier};
			if (identifier == "map") return {TokenType::Map, identifier};
			if (identifier == "set") return {TokenType::Set, identifier};
			continue;
		}
	}
	return {TokenType::Identifier, identifier};
}

std::vector<Token> Lexer::tokenize() {
	std::vector<Token> tokens;
	std::string identifier;
	int is_digit = 0, is_float = 0, alpha = 0, is_comment = 0, is_string = 0;
	for (unsigned long int i = 0; i < src.size(); i++) {
		char c = src[i];
		if (is_comment) {
			if (src[i - 1] == '*' && c == '/') {
				identifier = "*/";
				tokens.push_back({TokenType::Comment, identifier});
				identifier = "";
				is_comment = 0;
				continue;
			} else if (src[i - 1] == '*' && c != '/') {
				identifier += src[i - 1] + c;
				tokens.push_back({TokenType::BinaryOperator, "*"});
				continue;
			} else identifier += c;
		}
		if (isspace(identifier[0])) identifier = "";
		if (identifier == "") {
			if (!isspace(c)) { identifier += c; }
			if (isdigit(c)) is_digit = 1;
			if (isalpha(c)) alpha = 1;
			if (is_string) {
				if ((c == identifier[0]) && (src[i - 1] != '\\') &&
					(identifier.size() > 1)) {
					identifier += c;
					tokens.push_back({TokenType::String, identifier});
					identifier = "";
					is_string = 0;
				} else if (c == ' ') identifier += ' ';
				else if (isspace(c)) identifier += '\\' + c;
				else identifier += c;
				continue;
			} else {
				if (identifier[0] == '"' || identifier[0] == '\'') {
					std::cout << "Yes\n";
					is_string = 1;
					continue;
				}
			}
			continue;
		}
		if (is_digit && c == '.' && !is_float) {
			is_float = 1;
			identifier += c;
			continue;
		}
		if (is_digit && c == '.' && is_float) {
			tokens.push_back(strNum_to_token(identifier, is_float));
			is_digit = 0;
			is_float = 0;
			identifier = "";
			tokens.push_back({TokenType::MemberAccess, "."});
			continue;
		}
		if (isspace(identifier[0])) {
			identifier = "";
			continue;
		}
		if (alpha && (isalpha(c) || isdigit(c) || c == '_')) {
			identifier += c;
			continue;
		}
		if (is_digit && (isdigit(c) || c == '_')) {
			if (c != '_') identifier += c;
			continue;
		}
		if (isspace(c)) {
			if (alpha) {
				tokens.push_back(strKeyIden_to_token(identifier));
				alpha = 0;
			} else if (is_digit) {
				tokens.push_back(strNum_to_token(identifier, is_float));
				is_digit = 0;
				is_float = 0;
			} else {
				identifier = trim(identifier);
				if (Find(allSymbols, identifier) != 1) {
					if (Find(singleSymbol, identifier[0]) != 0) {
						if (identifier == "[")
							tokens.push_back({TokenType::OpenSquare, "["});
						else if (identifier == "]")
							tokens.push_back({TokenType::CloseSquare, "["});
						else if (identifier == "(")
							tokens.push_back({TokenType::OpenParen, "("});
						else if (identifier == ")")
							tokens.push_back({TokenType::OpenParen, ")"});
						else if (identifier == "{")
							tokens.push_back({TokenType::OpenParen, "{"});
						else if (identifier == "}")
							tokens.push_back({TokenType::OpenParen, "}"});
						else if (identifier == ",")
							tokens.push_back({TokenType::NextIdentifier, ","});
						else if (identifier == ",")
							tokens.push_back({TokenType::NextIdentifier, ","});
					} else if (Find(unarySym, identifier)) {
						tokens.push_back(
							{TokenType::UrinaryOperator, identifier});
					} else if (Find(binarySym, identifier)) {
						tokens.push_back(
							{TokenType::BinaryOperator, identifier});
					}
				} else {
					tokens.push_back({TokenType::Invalid, identifier});
				}
			}
			identifier = "";
			continue;
		}
	}
	return tokens;
}

std::ostream &operator<<(std::ostream &os, const Token &token) {
	os << keySwitch[token.type] << " " << token.literal;
	return os;
}
std::ostream &operator<<(std::ostream &os, const std::vector<Token> &tokens) {
	for (auto i = tokens.begin(); i != tokens.end(); i++) {
		os << (J7::Token)*i << "\n";
	}
	return os;
}
} // namespace J7
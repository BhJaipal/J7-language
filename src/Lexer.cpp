#include "Lexer.hpp"
#include "utils/Errors.hpp"

namespace J7 {

std::vector<Token> &tokenize(const std::string &source) {
	static std::vector<Token> tokens;
	tokens.clear();
	std::string remaining = source;
	return tokens;
}

std::ostream &operator<<(std::ostream &os, const Token &token) {
	std::string value =
		token.type == TokenType::EOLine ? "EOLine;" : token.literal;
	os << "Token {type: " << static_cast<int>(token.type)
	   << ", value: " << value << "}\n";
	return os;
}

std::ostream &operator<<(std::ostream &os, const std::vector<Token> &tokens) {
	for (auto &token : tokens) os << token;
	return os;
}

} // namespace J7
#include "lexer.hpp"
#include <map>
#include <string>

namespace J7 {

std::ostream &operator<<(std::ostream &os, const Token &token) {
	os << (std::uint8_t)token.type << " " << token.literal;
	return os;
}
std::ostream &operator<<(std::ostream &os, const std::vector<Token> &tokens) {
	for (auto i = tokens.begin(); i != tokens.end(); i++) {
		os << (J7::Token)*i << "\n";
	}
	return os;
}
} // namespace J7
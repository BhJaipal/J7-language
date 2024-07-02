#pragma once
#include <string>

namespace J7Util {
class RegExp {
  public:
	std::string _matcher, _flags;
	RegExp(std::string matcher, std::string flags)
		: _matcher(matcher), _flags(flags) {}
	RegExp(std::string matcher) : _matcher(matcher), _flags("") {}
};
} // namespace J7Util
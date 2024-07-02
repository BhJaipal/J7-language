#pragma once
#include "regexp.hpp"
#include <string>

namespace J7Util {
class String : public std::string {
  public:
	bool match(RegExp expression) {}
};
} // namespace J7Util
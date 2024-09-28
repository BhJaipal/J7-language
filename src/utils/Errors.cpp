/**
 * @file Errors.cpp
 * @author BhJaipal
 * @brief Source file for utils/Errors.cpp
 * @warning I won't warn again, it looks terrible but isn't that hard, still can
 *cause headache
 */

#include "utils/Errors.hpp"
#include <cstdlib>
#include <iostream>

namespace J7 {
// Error class
Error::Error(std::string message)
	: message_(message), line_content_(""), line_no_(0), name_("Error") {}
Error::Error(std::string name, std::string message)
	: message_(message), line_content_(""), line_no_(0), name_(name) {}
Error::Error(std::string name, std::string message, unsigned int line_no,
			 std::string line_content)
	: message_(message), line_content_(line_content), line_no_(line_no),
	  name_(name) {}

// SyntaxError
SyntaxError::SyntaxError(std::string message) : Error("SyntaxError", message) {}
SyntaxError::SyntaxError(std::string message, unsigned int line_no,
						 std::string line_content)
	: Error("SyntaxError", message, line_no, line_content) {}

// Zero Division Error
ZeroDivisionError::ZeroDivisionError()
	: Error("ZeroDivisionError",
			"Cannot divide number from 0, it results to Infinity") {}
ZeroDivisionError::ZeroDivisionError(unsigned int line_no,
									 std::string line_content)
	: Error("ZeroDivisionError",
			"Cannot divide number from 0, it results to Infinity", line_no,
			line_content) {}

// Type Error
TypeError::TypeError(std::string t1, std::string t2)
	: Error("TypeError", "Cannot convert from " + t1 + " to " + t2) {}
TypeError::TypeError(std::string t1, std::string t2, unsigned int line_no,
					 std::string line_content)
	: Error("TypeError", "Cannot convert from " + t1 + " to " + t2, line_no,
			line_content) {}

std::ostream &operator<<(std::ostream &os, const Error *error) {
	std::system((std::string("echo '\033[1;31m") + error->name_ +
				 std::string(": \033[0m'") + error->message_)
					.c_str());
	if (error->line_no_)
		os << error->line_no_ << " |    " << error->line_content_
		   << ((error->line_content_[error->line_content_.size() - 1] == '\n')
				   ? ' '
				   : '\n');
	return os;
}

} // namespace J7

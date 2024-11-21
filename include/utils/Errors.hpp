/**
 * @file Errors.hpp
 * @author BhJaipal
 * @brief Error for J7
 * @warning Open src/utils/Errors.cpp at your own risk, may cause brain rot and
 *head ache
 */
#pragma once
#include <string>

namespace J7 {
class Error {
  public:
	std::string message_;
	std::string line_content_;
	std::size_t line_no_;
	std::string name_;
	Error(std::string message_);
	Error(std::string name, std::string message);
	Error(std::string name, std::string message, std::size_t, std::string);
	friend std::ostream &operator<<(std::ostream &os, const Error *error);
};
class SyntaxError : public Error {
  public:
	SyntaxError(std::string message);
	SyntaxError(std::string message, std::size_t, std::string);
};
class ZeroDivisionError : public Error {
  public:
	ZeroDivisionError();
	ZeroDivisionError(std::size_t line_no, std::string line_content);
};
class TypeError : public Error {
  public:
	TypeError(std::string t1, std::string t2);
	TypeError(std::string t1, std::string t2, std::size_t line_no,
			  std::string line_content);
};

std::ostream &operator<<(std::ostream &os, const Error *error);
} // namespace J7

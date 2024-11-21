#pragma once
#include <string>
#include <vector>

namespace J7 {
int Find(std::string str, char c);
int Find(std::string str, std::string c);
int Find(std::vector<char> arr, char c);
int Find(std::vector<std::string> arr, std::string c);
int Find(std::vector<const char *> arr, std::string c);
} // namespace J7

#include "utils/Find.hpp"
#include <string>
#include <vector>

int J7::Find(std::string str, char c) {
	for (std::size_t i = 0; i < str.size(); i++) {
		if (str[i] == c) return i;
	}
	return -1;
}
int J7::Find(std::string str, std::string c) {
	for (std::size_t i = 0; i < str.size(); i++) {
		if (str[i] == c[0]) return i;
	}
	return -1;
}
int J7::Find(std::vector<char> arr, char c) {
	for (std::size_t i = 0; i < arr.size(); i++) {
		if (arr[i] == c) return i;
	}
	return -1;
}
int J7::Find(std::vector<std::string> arr, std::string c) {
	for (std::size_t i = 0; i < arr.size(); i++) {
		if (arr[i] == c) return i;
	}
	return -1;
}
int J7::Find(std::vector<const char *> arr, std::string c) {
	for (std::size_t i = 0; i < arr.size(); i++) {
		if (arr[i] == c) return i;
	}
	return -1;
}

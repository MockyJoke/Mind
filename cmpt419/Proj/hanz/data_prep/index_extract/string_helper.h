#pragma once
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <vector>
#include <sstream>

// trim from start
static inline std::string &ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(),
		std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(),
		std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
	return ltrim(rtrim(s));
}

//
static inline std::string getFirstWord(std::string& s) {
	size_t endPos = s.find_first_of(' ');
	std::string word = s.substr(0, endPos);
	return word;
}

// split string
std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> strings;
	std::istringstream f(s);
	std::string a;
	while (std::getline(f, a, delim)) {
		strings.push_back(a);
	}
	return strings;
}

std::string replaceString(std::string subject, const std::string& search,
	const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

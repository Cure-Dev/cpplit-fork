#pragma once

#include <map>
#include <string>

class node {

public:
	int BEGIN;
	int END;
	virtual std::wstring view() = 0;

	node(int begin, int end) : BEGIN(begin), END(end) {};

};

// #include <regex>
static std::wstring block_indent(std::wstring ori) {
/*	std::wregex pat (L"\n");
	std::wstring result = L"\t" + std::regex_replace(ori, pat, L"\n\t");
	result = result.substr(0, result.length()-1); // rm the last \t
	return result;*/
	return ori;
}
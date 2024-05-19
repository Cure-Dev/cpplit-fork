#pragma once

#include "./token.hpp"

class token_entity : public token {
public:
	token_entity(int begin, int end) : token(begin, end) {};
};

class token_identifier : public token_entity {
public:
	std::wstring VALUE; // name
	token_identifier(std::wstring value, int begin, int end) : token_entity(begin, end), VALUE(value) {};

	std::wstring view() {
		return this->VALUE;
	};
};

class token_literal : public token_entity {
public:
	token_literal(int begin, int end) : token_entity(begin, end) {};
};

class token_number : public token_literal {
public:
	token_number(int val, int begin, int end) : token_literal(begin, end), val(val) {};
	int val;

	std::wstring view() {
		return std::to_wstring(this->val);
	};
};

class token_string : public token_literal {
public:
	std::wstring VALUE;
	token_string(std::wstring value, int begin, int end) : token_literal(begin, end), VALUE(value) {};

	std::wstring view() {
/*		static std::map<wchar_t, std::wstring> replace_map = {
			{ L'\n', L"`U+000A`"},
		};*/
		return L"'" + this->VALUE + L"'";
	};
};

class token_boolean : public token_literal {
public:
	bool VALUE;
	token_boolean(bool val, int begin, int end) : token_literal(begin, end), VALUE(val) {};

	std::wstring view() {
		std::wstring result = L"\033[34m";
		if (this->VALUE) {
			result += L"true";
		}
		else {
			result += L"false";
		}
		result += L"\033[0m";
		return result;
	}
};
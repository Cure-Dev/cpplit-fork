#pragma once

#include "./expr.hpp"

class func : public expr {
public:
	func(std::vector<std::pair<token_identifier*, std::vector<expr*>>> arglist, std::vector<expr*> return_types, statement* body, int begin, int end) : expr(begin, end), arglist(arglist), return_types(return_types), body(body) {};
	std::vector<std::pair<token_identifier*, std::vector<expr*>>> arglist;
	std::vector<expr*> return_types;
	statement* body;

	std::wstring view() {
		std::wstring result;
		result += L"func(";
		for (auto arg : this->arglist) {
			result += arg.first->VALUE + L"->(";
			for (auto type : arg.second) {
				result += type->view() + L", ";
			}
			result += L")";
		}
		result += L" -> (";
		for (auto type : this->return_types) {
			result += type->view() + L", ";
		}
		result += L") {\n";
		result += this->body->view();
		result += L"}";
		return result;
	}
};
#pragma once

#include "./statement.hpp"
#include "ast/expressions/expr.hpp"

class defination : public statement {
public:
	defination(token_identifier* Identifier, expr* Val, int begin, int end) : statement(begin, end), IDENTIFIER(Identifier), VALUE(Val) {}; // 'id' to 'ref'
	token_identifier* IDENTIFIER;
	expr* VALUE;

	std::wstring view() {
		return statement::statement_view(this->IDENTIFIER->view() + L" = " + this->VALUE->view());
	}

	void exec(environment& env, const runtime& Runtime) {
		env.add(this->IDENTIFIER->VALUE, this->VALUE->eval(env, Runtime));
	}
};
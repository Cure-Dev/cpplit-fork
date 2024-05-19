#pragma once

#include "ast/expressions/expr.hpp"
#include "ast/statements/statement.hpp"
#include "ast/statements/statement_block.hpp"

#include "utils/call_method.hpp"

class condition_loop : public statement {
public:
	condition_loop(expr* Condition, statement_block* Block, int begin, int end) : statement(begin, end), CONDITION(Condition), BODY(Block) {};
	expr* CONDITION;
	statement_block* BODY;

	std::wstring view() {
		return L"loop (" + this->CONDITION->view() + L") {\n" + block_indent(this->BODY->view()) + L"}\n";
	}

	void exec(environment env, const runtime& Runtime) {
		while (dynamic_cast<builtin_object_bool*> (call_method(dynamic_cast<semantic_object*> (this->CONDITION->eval(env, Runtime)), L"bool")) -> data) {
			this->BODY->exec(env, Runtime);
		}
	};
};
#pragma once

#include "ast/expressions/expr.hpp"
#include "ast/statements/statement.hpp"
#include "ast/statements/statement_block.hpp"
#include "utils/call_method.hpp"

class conditional_branch : public statement {
public:
	conditional_branch(expr* Condition, statement_block* Block, int begin, int end) : statement(begin, end), CONDITION(Condition), BLOCK(Block) {};

	expr* CONDITION;
	statement_block* BLOCK;

	std::wstring view() {
		return L"(" + this->CONDITION->view() + L") {\n" + block_indent(this->BLOCK->view()) + L"}\n";
	}

};

class conditional_branches : public statement {
public:
	conditional_branches(std::vector<conditional_branch*> Branches, int begin, int end) : statement(begin, end), BRANCHES(Branches) {};

	std::vector<conditional_branch*> BRANCHES;

	std::wstring view() {
		std::wstring result;

		result += L"if " + this->BRANCHES[0]->view();

		int l = this->BRANCHES.size();
		for (int i = 1; i < l; i += 1) {
			result += L"else if " + this->BRANCHES[i]->view();
		}

		return result;
	}

	void exec(environment env, const runtime& Runtime) {
		for (auto branch : this->BRANCHES) {
			semantic_object* condition = dynamic_cast<semantic_object*> (branch->CONDITION->eval(env, Runtime));
			semantic_object* interface;

			try {
				interface = call_method(condition, L"bool");
			}
			catch (const char*) {
				throw "no impl: bool()";
			}

			bool result = dynamic_cast<builtin_object_bool*> (interface) -> data;

			if (result) {
				branch->BLOCK->exec(env, Runtime);
				break;
			}
		}
	};
};
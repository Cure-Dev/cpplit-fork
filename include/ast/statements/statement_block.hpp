#pragma once

#include "./statement.hpp"
#include "./defination.hpp"

class statement_block : public statement {
public:
	statement_block(std::vector<statement*> block, int begin, int end) : statement(begin, end), BLOCK(block) {};
	std::vector<statement*> BLOCK;

	std::wstring view() {
		std::wstring result;
		for (auto Statement : this->BLOCK) {
			result += Statement->view();
		}
		return result;
	}

	void exec(environment env, const runtime& Runtime) {
		for (auto Statement : this->BLOCK) {
			if (dynamic_cast<defination*> (Statement) != NULL) {
				(dynamic_cast<defination*> (Statement))->exec(env, Runtime);
			}
			else {
				Statement->exec(env, Runtime);
			}
		}
	}
};
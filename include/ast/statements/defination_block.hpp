#pragma once

#include "./statement.hpp"
#include "./defination.hpp"

class defination_block : public statement {
public:
	defination_block(std::vector<defination*> block, int begin, int end) : statement(begin, end), BLOCK(block) {};

	std::vector<defination*> BLOCK;

	std::wstring view() {
		std::wstring result;

		for (auto Defination : this->BLOCK) {
			result += Defination->view();
		}
		return result;
	}
};

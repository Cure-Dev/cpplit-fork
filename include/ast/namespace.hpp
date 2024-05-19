#pragma once

#include "./statements/defination_block.hpp"

class TYPE_namespace : public expr {
public:
	TYPE_namespace(defination_block* block, int begin, int end) : expr(begin, end), BLOCK(block) {};
	defination_block* BLOCK;

	std::wstring view() {
		return L"namespace {\n" + block_indent(this->BLOCK->view()) + L"}";
	}
};
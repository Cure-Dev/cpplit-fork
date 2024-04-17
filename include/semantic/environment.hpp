#pragma once

#include <string>

#include "semantic/class.hpp"
#include "exceptions/runtime_errors.hpp"


class environment { // 符号表
public:
	environment() {};

/*	semantic_node* operator[](std::wstring name) {
		return this[name];
	}
*/
	// declare define assign
	// insert
	void add(std::wstring name, semantic_node* val) {
		this->map[name] = val;
	}

	// find / get
	semantic_node* get(std::wstring name) {
		if (this->map.find(name) != this->map.end()) {
			return this->map[name];
		}
		else {
			throw new undefined_identifier { name };
		}
	}

private:
	std::map<std::wstring, semantic_node*> map;
};
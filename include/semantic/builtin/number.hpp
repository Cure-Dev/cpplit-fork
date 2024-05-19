#pragma once

#include <gmpxx.h>

#include "semantic/class.hpp"
#include "semantic/object.hpp"
#include "semantic/builtin/string.hpp"

class _semantic_builtin_class_integer : public semantic_class {
public:
	_semantic_builtin_class_integer() {};
	semantic_node* get_member(std::wstring name) {
		throw "no member in null";
	}
};

static auto semantic_builtin_class_integer = new _semantic_builtin_class_integer {};

class semantic_builtin_object_integer : public semantic_object {
public:
	semantic_builtin_object_integer(int val) : semantic_object(semantic_builtin_class_integer, {}), val(val) {};
	int val;
	std::wstring view_temp;/**/

	semantic_object* get_member(std::wstring name) {
		if (name == L"output") {
			return new builtin_object_string { std::to_wstring(this->val) };
		}
		else if (name == L"string") {
			throw "not support";
		}
		else {
			throw "member not exists.";
		}
	}
};
#pragma once

#include "semantic/class.hpp"
#include "semantic/object.hpp"

class _semantic_builtin_class_tuple : public semantic_class {
public:
	_semantic_builtin_class_tuple() {};
	semantic_node* get_member(std::wstring name) {
		throw "no member in tuple"; // init
	}
};

static auto semantic_builtin_class_tuple = new _semantic_builtin_class_tuple {};

class _builtin_object_tuple : public semantic_object {
public:
	_builtin_object_tuple(std::vector<semantic_object*> items) : semantic_object(semantic_builtin_class_tuple, {}), items(items) {};
	std::vector<semantic_object*> items;
};
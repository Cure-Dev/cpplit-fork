#pragma once

#include "./node.hpp"
#include "./object.hpp"
#include "semantic/builtin/tuple.hpp"
class semantic_function : public semantic_node {
public:
	semantic_function(semantic_class* type) : return_type(type) {};
	semantic_class* return_type;

	virtual semantic_object* call(_builtin_object_tuple* arglist) {
		return this->call(arglist);
	}

	semantic_node* get_member(std::wstring name) {
	/*	if (name == L"call") {

		}*/
		return NULL;
	}
};

class semantic_method : public semantic_node {
public:
	semantic_method(semantic_class* type) : return_type(type) {};
	semantic_class* return_type;
	virtual semantic_object* call(semantic_object* obj, std::vector<semantic_object*> arglist) {
		throw "no call";
	};

	semantic_node* get_member(std::wstring name) {
		throw L"no member named " + name;
	}
};

// null == null
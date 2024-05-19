#pragma once

#include "semantic/object.hpp"

static class : public semantic_class {
public:
	semantic_node* get_member(std::wstring name) {
		throw "no member in null";
	}
} *semantic_builtin_class_null = {};

static auto semantic_builtin_object_null = new semantic_object { semantic_builtin_class_null, {} };
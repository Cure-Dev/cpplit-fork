#pragma once

#include "semantic/class.hpp"
#include "semantic/object.hpp"

extern semantic_class* builtin_class_string;

class builtin_object_string : public semantic_object {
public:
	builtin_object_string(std::wstring data) : semantic_object(builtin_class_string, {}), val(data) {};
	std::wstring val;
};
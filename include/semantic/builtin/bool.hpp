#pragma once

#include "semantic/class.hpp"
#include "semantic/object.hpp"

extern semantic_class* builtin_class_bool;

class builtin_object_bool : public semantic_object {
public:
	builtin_object_bool(bool data) : semantic_object(builtin_class_bool, {}), data(data) {};
	bool data;
};
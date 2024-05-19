#pragma once

#include "semantic/class.hpp"
#include "semantic/fm.hpp"
#include "semantic/builtin/null.hpp"
#include "semantic/builtin/string.hpp"

#include <iostream>


class _lit_io_output : public semantic_function {
public:
	_lit_io_output() : semantic_function(semantic_builtin_class_null) {};

	semantic_object* call(_builtin_object_tuple* arglist) {
		auto arg = arglist->items[0];
		auto _out = arg->get_member(L"output");
		if (auto out = dynamic_cast <builtin_object_string*> (_out); out != NULL) {//!
			std::wcout << out -> val;
		}
		else if (auto out = dynamic_cast <semantic_method*> (_out); out != NULL) {
			std::wcout << dynamic_cast <builtin_object_string*> (out->call(arg, {})) -> val;
		}
		return semantic_builtin_object_null;
	}
};

static _lit_io_output* lit_io_output = new _lit_io_output {};
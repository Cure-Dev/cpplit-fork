#include "semantic/fm.hpp"
#include "semantic/builtin/string.hpp"

class _string_init : public semantic_function {
public:
	_string_init() : semantic_function(builtin_class_string) {};

	semantic_object* call(_builtin_object_tuple* arglist) {
		auto arg = arglist->items[0];
		return (dynamic_cast<semantic_method*> (arg->get_member(L"string"))) -> call(arg, {});
	}
} *string_init = new _string_init {};
/*不写new _string_init 无法初始化？*/

class _string_string : public semantic_method {
public:
	_string_string() : semantic_method(builtin_class_string) {};

	semantic_object* call(semantic_object* obj, std::vector<semantic_object*> arglist) {
		return obj;
	}
};
auto string_string = new _string_string {};

class _string_output : public semantic_method {
public:
	_string_output() : semantic_method(builtin_class_string) {};

	semantic_object* call(semantic_object* obj, std::vector<semantic_object*> arglist) {
		return obj;
	}
};
auto string_output = new _string_output {};

class _string_add : public semantic_method {
public:
	_string_add() : semantic_method(builtin_class_string) {}; //!

	semantic_object* call(semantic_object* obj, std::vector<semantic_object*> arglist) {
		auto left = dynamic_cast<builtin_object_string*> (obj);
		auto _right = arglist[0];
		auto right = dynamic_cast<builtin_object_string*> (dynamic_cast<semantic_method*> (_right->get_member(L"string")) -> call(_right, {}));
		return new builtin_object_string { left->val + right->val };
	}
};
auto string_add = new _string_add {};

semantic_class* builtin_class_string = new semantic_class { {}, 
	{{L"call", string_init}}, 
	{
		{ L"string", string_string },
		{ L"output", string_output },
		{ L"add", string_add },
	}
};
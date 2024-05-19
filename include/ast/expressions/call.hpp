#pragma once

class arg_list : public paren_list {
public:
	arg_list(std::vector<expr*> items, int begin, int end) : paren_list(items, begin, end) {};

	_builtin_object_tuple* eval(environment env, const runtime& Runtime) {
		std::vector<semantic_object*> result;
		for (auto item : this->ITEMS) {
			result.push_back(dynamic_cast<semantic_object*> ((item)->eval(env, Runtime)));
		}
		return new _builtin_object_tuple { result };
	}
};

class expr_callation : public expr {
public:
	expr_callation(expr* callee, arg_list* caller, int begin, int end) : expr(begin, end), CALLEE(callee), CALLER(caller) {};
	expr* CALLEE;
	arg_list* CALLER; // expr_iter: use abs class?

	std::wstring view() {
		return this->CALLEE->view() + this->CALLER->view();
	}

	semantic_node* eval(environment env, const runtime& Runtime) {
		auto callee = this->CALLEE->eval(env, Runtime);

		if (dynamic_cast<semantic_function*> (callee) != NULL) { //
			return (dynamic_cast<semantic_function*> (callee)) -> call(this->CALLER->eval(env, Runtime));
		}
		else if (auto call = dynamic_cast<semantic_class*> (callee); call != NULL) {//!
			return dynamic_cast<semantic_function*> (call->members[L"call"]) -> call(this->CALLER->eval(env, Runtime));
		}
		else {
			throw "cannot call";
		}
	}
};
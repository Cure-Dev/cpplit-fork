#pragma once

#include <string>
#include <vector>

#include "tokens/entity.hpp"

#include "ast/node.hpp"

#include "semantic/class.hpp"
#include "semantic/builtin/number.hpp"
#include "semantic/environment.hpp"
#include "semantic/builtin/bool.hpp"

#include "runtime.hpp"

class expr : public node {
public:
	expr(int begin, int end) : node(begin, end) {};

	virtual semantic_node* eval(environment env, const runtime& Runtime) { // 忘写virtual
		throw "pure virtual method";
		// return this->eval(env); //
	};

};

// --- entity --- //
class expr_entity : public expr {
public:
	expr_entity(int begin, int end) : expr(begin, end) {};
};


class expr_number : public expr_entity {
public:
	expr_number(token_number* val, int begin, int end) : expr_entity(begin, end), VALUE(val) {};

	token_number* VALUE;

	std::wstring view() {
		return this->VALUE->view();
	}

	semantic_node* eval(environment env, const runtime& Runtime) {
		return new semantic_builtin_object_integer { this->VALUE->val };
	}
};

class expr_string : public expr_entity {
public:
	expr_string(token_string* val, int begin, int end) : expr_entity(begin, end), VALUE(val) {};

	token_string* VALUE;

	std::wstring view() {
		return this->VALUE->view();
	}

	semantic_node* eval(environment env, const runtime& Runtime) {
		return new builtin_object_string { this->VALUE->VALUE };
	}
};

class expr_boolean : public expr_entity {
public:
	expr_boolean(token_boolean* val, int begin, int end) : expr_entity(begin, end), VALUE(val) {};

	token_boolean* VALUE;

	std::wstring view() {
		return this->VALUE->view();
	}

	semantic_node* eval(environment env, const runtime& Runtime) {
		return new builtin_object_bool { this->VALUE->VALUE };
	}
};

class expr_identity : public expr_entity {
public:
	expr_identity(int begin, int end) : expr_entity(begin, end) {};
};

class expr_identifier : public expr_identity {
public:
	expr_identifier(token_identifier* val, int begin, int end) : expr_identity(begin, end), VALUE(val) {};

	token_identifier* VALUE; // name?

	std::wstring view() {
		return this->VALUE->view();
	}

	semantic_node* eval(environment env, const runtime& Runtime) {
		return env.get(this->VALUE->VALUE);
	}
};

class expr_iter : public expr { // node
public:
	std::vector<expr*> ITEMS;
	expr_iter(std::vector<expr*> items, int begin, int end) : expr(begin, end), ITEMS(items) {};
};

class paren_list : public expr_iter {
public:
	paren_list(std::vector<expr*> items, int begin, int end) : expr_iter(items, begin, end) {};

	std::wstring view() {
		std::wstring result = L"(";
		for (expr* item : this->ITEMS) {
			result += item->view() + L", ";
		}
		result += L")";
		return result;
	}

};
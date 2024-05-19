#pragma once

#include "ast/node.hpp"
#include "semantic/environment.hpp"
#include "runtime.hpp"

class statement : public node {
public:
	statement(int begin, int end) : node(begin, end) {};

	std::wstring statement_view(std::wstring s) {
		return s + L";\n";
	}

	virtual void exec(environment env, const runtime& Runtime) {}; //!

};
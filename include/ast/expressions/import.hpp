#pragma once

#include "semantic/class.hpp"
#include "semantic/environment.hpp"

#include "utils/path.hpp"
#include "utils/project.hpp"

#include "runtime.hpp"

#include "ast/expressions/expr.hpp"

class import : public expr {
public:
	import(expr_string* Path, int begin, int end) : expr(begin, end), Path(path {Path->VALUE->VALUE}) {};

	path Path;

	std::wstring view() {
		std::wstring Path;
		for (auto session : this->Path.data) {
			Path += session + L'/';
		}
		return L"import (" + Path + L")";
	}

	semantic_node* eval(environment env, const runtime& Runtime) {

		project* result = &all;

		for (std::wstring name : this->Path.data) {
			if (dynamic_cast<library*> (result) != NULL) {
				result = dynamic_cast<library*> (result) -> access(name);
			}
			else {
				throw name + L" is a module, not library.";
			}
		}

		if (dynamic_cast<module*> (result) != NULL) {
			return dynamic_cast<module*> (result)->eval();
		}
		else {
			throw "not module";
		}
	}
};
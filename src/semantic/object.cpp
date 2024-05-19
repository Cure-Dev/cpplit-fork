#include "semantic/object.hpp"
#include "semantic/fm.hpp"

semantic_node* semantic_object::get_member(std::wstring name) {
	if (this->members.find(name) != this->members.end()) {
		return this->members[name];
	}
	else if (this->type->methods.find(name) != this->type->methods.end()) {
		return this->type->methods[name];
	}
	else {
		throw L"no member named " + name;
	}
}
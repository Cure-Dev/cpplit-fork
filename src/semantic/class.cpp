#include "semantic/class.hpp"

semantic_node* semantic_class::get_member(std::wstring name) {
	if (this->members.find(name) != this->members.end()) {
		return this->members[name];
	}
	else {
		throw "member not exists.";
	}
}
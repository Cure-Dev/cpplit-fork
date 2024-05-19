#pragma once

#include <vector>
#include <map>
#include <string>

#include "./node.hpp"

class semantic_method;

class semantic_class : public semantic_node {
public:
	semantic_class(std::vector<semantic_class*> supers = {}, std::map<std::wstring, semantic_node*> members = {}, std::map<std::wstring, semantic_method*> methods = {}) : semantic_node(members), supers(supers), methods(methods) {};
	std::vector<semantic_class*> supers;
	std::map<std::wstring, semantic_method*> methods;

	semantic_node* get_member(std::wstring name);
};
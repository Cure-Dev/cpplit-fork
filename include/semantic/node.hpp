/* data model */

#pragma once

#include <map>
#include <string>

class semantic_node {
public:
	semantic_node(std::map<std::wstring, semantic_node*> members = {}) : members(members) {};
	virtual semantic_node* get_member(std::wstring name) = 0;
	std::map<std::wstring, semantic_node*> members;
};
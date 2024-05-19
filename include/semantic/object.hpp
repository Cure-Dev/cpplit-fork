#pragma once

#include <map>
#include <string>

#include "./node.hpp"
#include "./class.hpp"

class semantic_object : public semantic_node {
public:
	semantic_object(semantic_class* type, std::map<std::wstring, semantic_object*> members) : type(type), members(members) {};
	semantic_class* type;
	semantic_node* get_member(std::wstring name);

private:
	std::map<std::wstring, semantic_object*> members;
};
#pragma once

#include <filesystem>

#include "semantic/builtin/io/output.hpp"
#include "semantic/builtin/bool.hpp"
#include "semantic/builtin/string.hpp"

class project {
public:
	virtual ~project() {};
};

class module : public project {
public:
	virtual semantic_node* eval(/*runtime*/) = 0;
};

class library : public project {
public:
	virtual project* access(std::wstring name) = 0;
};


class custom_library : public library {
public:
	custom_library(std::filesystem::path Path) : Path(Path) {};
	std::filesystem::path Path;

	project* access(std::wstring name) {
		throw "";
	}

};

class root_library : public library {
public:
	root_library(std::map<std::wstring, project*> subs) : subs(subs) {};
	std::map<std::wstring, project*> subs;

	project* access(std::wstring name) {
		if (this->subs.find(name) != this->subs.end()) {
			return this->subs[name];
		}
		else {
			throw "project not found";
		}
	}
};

// ------

static class : public module {
public:

	semantic_node* eval() {
		return lit_io_output;
	}

} mod_lit_io_output;

static class : public module {
public:
	semantic_node* eval() {
		return builtin_class_bool;
	}
} mod_bool;

static class : public module {
public:
	semantic_node* eval() {
		return builtin_class_string;
	}
} mod_string;

static class : public library {
public:
	project* access(std::wstring name) {
		if (name == L"output") {
			return &mod_lit_io_output;
		}
		else {
			throw "no member";
		}
	}
} lib_lit_io;

static class : public library {
public:
	project* access(std::wstring name) {
		if (name == L"io") {
			return &lib_lit_io;
		}
		else if (name == L"bool") {
			return &mod_bool;
		}
		else if (name == L"string") {
			return &mod_string;
		}
		else {
			throw L"no member named " + name;
		}
	}
} lib_lit; 

static class : public library {
public:
	project* access(std::wstring name) {
		if (name == L"lit" || name == L"光") {
			return &lib_lit;
		}
		else {
			throw L"no member named " + name;
		}
	}
} all; //! temp
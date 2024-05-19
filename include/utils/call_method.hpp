#pragma once

#include <string>
#include "semantic/fm.hpp"

semantic_object* call_method(semantic_object* obj, std::wstring name, std::vector<semantic_object*> arglist = {});
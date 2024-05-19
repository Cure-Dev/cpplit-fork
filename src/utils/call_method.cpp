#include "utils/call_method.hpp"

semantic_object* call_method(semantic_object* obj, std::wstring name, std::vector<semantic_object*> arglist) {
	if (auto method = dynamic_cast<semantic_method*> (obj->get_member(name)); method != NULL) {
		return method->call(obj, arglist);
	}
	else {
		throw "not method";
	}
}
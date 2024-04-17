#pragma once

class member_access : public expr_identity {
public:
	member_access(expr* obj, token_identifier* mem, int begin, int end) : expr_identity(begin, end), OBJECT(obj), MEMBER(mem) {};
	expr* OBJECT;
	token_identifier* MEMBER;

	std::wstring view() {
		return L"(" + this->OBJECT->view() + L"." + this->MEMBER->view() + L")";
	}
};
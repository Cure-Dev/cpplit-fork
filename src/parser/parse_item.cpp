#include "parser/parse_item.hpp"

#include "parser/parse_suffix.hpp"

#include "parser/parse_expression.hpp"
#include "parser/check.hpp"

expr_entity* make_entity(token_entity* Token) {
	
	if (dynamic_cast<token_number*> (Token) != NULL) {
		return new expr_number { dynamic_cast<token_number*> (Token), Token->BEGIN, Token->END }; // number unit
	}

	else if (dynamic_cast<token_string*> (Token) != NULL) {
		return new expr_string { dynamic_cast<token_string*> (Token), Token->BEGIN, Token->END };
	}

	else if (dynamic_cast<token_boolean*> (Token) != NULL) {
		return new expr_boolean { dynamic_cast<token_boolean*> (Token), Token->BEGIN, Token->END };
	}

	else if (dynamic_cast<token_identifier*> (Token) != NULL) {
		return new expr_identifier { dynamic_cast<token_identifier*> (Token), Token->BEGIN, Token->END }; // id string
	}

	else {
		throw "unknown entity"; //
	}

}

#include "parser/parse_import.hpp"
#include "parser/namespace/parse_namespace.hpp"
#include "parser/parse_func.hpp"

expr* parse_miditem(token_list& Token_list) {
	expr* result;

	token* first = Token_list.fetch();
	if (dynamic_cast<token_entity*> (first) != NULL) {
		result = make_entity(dynamic_cast<token_entity*> (first));
	}

	else if (dynamic_cast<token_keyword*> (first) != NULL) {

		token_keyword* kw = dynamic_cast<token_keyword*> (first);

		if (kw->Type == token_keyword::type::IMPORT) {
			result = parse_import(Token_list);
		}

		else if (kw->Type == token_keyword::type::NAMESPACE) {
			result = parse_namespace(Token_list);
		}

		else if (kw->Type == token_keyword::type::FUNC) {
			result = parse_func(Token_list);
		}

		else {
			throw "not expr kw";
		}
	}

	else if (check::symbol::is(first, token_symbol::type::PAREN_LEFT)) {
		result = parse_expression(Token_list);
		check::symbol::require(Token_list, token_symbol::type::PAREN_RIGHT);
	}

	else {
		throw "unknown token";
	}

	result = parse_suffix(Token_list, result);

	return result;
}


#include "parser/parse_func.hpp"
#include "parser/check.hpp"
#include "parser/parse_expression.hpp"
#include "parser/parse_statement_block.hpp"
#include "ast/expressions/func.hpp"

expr* parse_func(token_list& Token_list) {
	std::vector<std::pair<token_identifier*, std::vector<expr*>>> arglist;

	check::symbol::require(Token_list, token_symbol::type::PAREN_LEFT);

	while (!check::symbol::is(Token_list.this_(), token_symbol::type::PAREN_RIGHT)) {

		if (auto id = dynamic_cast<token_identifier*> (Token_list.fetch()); id != NULL) {
			check::symbol::require(Token_list, token_symbol::type::SINGLE_ARROW);
			expr* type = parse_expression(Token_list);

			auto after = Token_list.this_();
			if (check::symbol::is(after, token_symbol::type::COMMA)) {
				Token_list.next();
			}
			else if (check::symbol::is(after, token_symbol::type::PAREN_RIGHT)) {
				// pass
			}
			else {
				throw "expect seperator";
			}
			arglist.push_back({ id, { type } });
		}
		else {
			throw "expect identifier";
		}
	}
	Token_list.next();

	std::vector<expr*> return_types;

	if (check::symbol::is(Token_list.this_(), token_symbol::type::SINGLE_ARROW)) {
		Token_list.next();
		return_types.push_back(parse_expression(Token_list));
	}

	check::symbol::require(Token_list, token_symbol::type::BRACE_LEFT);
	auto body = parse_statement_block(Token_list);

	return new func { arglist, return_types, body, 0,0 };
}
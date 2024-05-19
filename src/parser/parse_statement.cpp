#include "parser/parse_expression.hpp"

#include "parser/parse_if.hpp"
#include "parser/loop/parse_loop.hpp"
#include "parser/check.hpp"

#include "ast/statements/expr_statement.hpp"

statement* parse_expr_statement(token_list& Token_list) {
	expr* Expr = parse_expression(Token_list);
	return new expr_statement { Expr, Expr->BEGIN, Expr->END };
}

statement* parse_statement(token_list& Token_list) {
	if (dynamic_cast<token_identifier*> (Token_list.this_()) != NULL && check::symbol::is(Token_list.peek(), token_symbol::type::EQUAL)) {
		token_identifier* idn = dynamic_cast<token_identifier*> (Token_list.fetch());
		Token_list.next();
		expr* val = parse_expression(Token_list); // a = +1
		return new defination { idn, val, idn->BEGIN, val->END };
	}

	else if (dynamic_cast<token_keyword*> (Token_list.this_()) != NULL) {

		token_keyword::type keyword = dynamic_cast<token_keyword*> (Token_list.this_()) -> Type;
		Token_list.next();

		if (keyword == token_keyword::type::IF) {
			return parse_ifs(Token_list);
		}

		else if (keyword == token_keyword::type::ELSE_IF) {
			throw "'else if' without privious 'if'";
		}

		else if (keyword == token_keyword::type::ELSE) {
			throw "'else' without privious 'if'";
		}

		else if (keyword == token_keyword::type::LOOP) {
			return parse_loop(Token_list);
		}

		else {
			Token_list.prev(); //!
			return parse_expr_statement(Token_list);
		}
	}

	else {
		return parse_expr_statement(Token_list);
	}
}
#include "ast/nodes.hpp"
#include "parser/parse_expression.hpp"
#include "parser/parse_statement_block.hpp"
#include "parser/check.hpp"

conditional_branch* parse_if(token_list& Token_list) {

	check::symbol::require(Token_list, token_symbol::type::PAREN_LEFT);
	expr* Condition = parse_expression(Token_list);
	check::symbol::require(Token_list, token_symbol::type::PAREN_RIGHT);

	check::symbol::require(Token_list, token_symbol::type::BRACE_LEFT);

	statement_block* block = parse_statement_block(Token_list);

	return new conditional_branch { Condition, block, Condition->BEGIN, block->END };
}

conditional_branch* parse_else(token_list& Token_list) {

	check::symbol::require(Token_list, token_symbol::type::BRACE_LEFT);

	statement_block* block = parse_statement_block(Token_list);

	return new conditional_branch { new expr_boolean { new token_boolean { true, 1, 1 }, 1, 1 }, block, 1, block->END };
}

conditional_branches* parse_ifs(token_list& Token_list) {

	std::vector<conditional_branch*> result;
	result.push_back(parse_if(Token_list));

	while (true) {
		if (check::symbol::is(Token_list.this_(), token_symbol::type::EOL_)) {
			Token_list.next();
		}
		else if (dynamic_cast<token_keyword*> (Token_list.this_()) != NULL && dynamic_cast<token_keyword*> (Token_list.this_())->Type == token_keyword::type::ELSE_IF) {
			Token_list.next();
			result.push_back(parse_if(Token_list));
		}
		else {
			break;
		}
	}

	if (dynamic_cast<token_keyword*> (Token_list.this_()) != NULL && dynamic_cast<token_keyword*> (Token_list.this_())->Type == token_keyword::type::ELSE) {
		Token_list.next();
		result.push_back(parse_else(Token_list));
	}

	return new conditional_branches { result, 1, 1 };
}
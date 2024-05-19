#pragma once

#include "ast/statements/statement_block.hpp"

// statement* parse_dec(token_list& Token_list); // declaration
statement_block* parse_exe(std::wstring filepath); // imperative
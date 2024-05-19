#pragma once

#include "tokens/token_list.hpp"
#include "ast/statements/defination.hpp"
#include "ast/statements/defination_block.hpp"

defination* parse_defination(token_list& Token_list);
defination_block* parse_defination_block(token_list& Token_list);
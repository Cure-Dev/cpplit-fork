#include "scanner/scanner.hpp"
#include "scanner/scan_string.hpp"

#include "utils/coding.hpp"
#include "reader/reader.hpp"

#include "tokens/tokens.hpp"
#include "utils/numcvt.hpp"
#include "exceptions/lex_errors.hpp"

#include "utils/ranges.hpp"
#include "utils/null.hpp"

#include <string>
#include <vector>
#include <map>

#include <regex>

#include "utils/position.hpp"
#include "scanner/trie.hpp"

ranges identifier_charset = {

	{ 65, 90 },  // A-Z
	{ 97, 122 }, // a-z
	{ 95, 95 },  // _
	// { 36, 36 },  // $
	{ 48, 57 },  // 0-9
	{ 32, 32 },  // ␣

	{ 0x4E00, 0x9FFF }, // chinese characters

};

static std::map<std::wstring, token_keyword::type> keyword_map = {

// en-us
	{ L"import", token_keyword::type::IMPORT },
	{ L"if", token_keyword::type::IF },
	{ L"else if", token_keyword::type::ELSE_IF },
	{ L"else", token_keyword::type::ELSE },
	{ L"loop", token_keyword::type::LOOP },
	{ L"func", token_keyword::type::FUNC },
	{ L"class", token_keyword::type::CLASS },
	{ L"namespace", token_keyword::type::NAMESPACE },
	{ L"enum", token_keyword::type::ENUM },
	{ L"try", token_keyword::type::TRY },
	{ L"catch", token_keyword::type::CATCH },

// zh-cn
	{ L"导入", token_keyword::type::IMPORT },
	{ L"如果", token_keyword::type::IF },
	{ L"若", token_keyword::type::IF },
	{ L"又如果", token_keyword::type::ELSE_IF },
	{ L"否则", token_keyword::type::ELSE },
	{ L"循环", token_keyword::type::LOOP },
	{ L"函数", token_keyword::type::FUNC },
	{ L"类", token_keyword::type::CLASS },
	{ L"命名空间", token_keyword::type::NAMESPACE },
	{ L"枚举", token_keyword::type::ENUM },
	{ L"尝试", token_keyword::type::TRY },
	{ L"捕获", token_keyword::type::CATCH },

};

static std::map<std::wstring, bool> literal_bool_map = {

// en-us
	{ L"true", true },
	{ L"false", false },

// zh-cn
	{ L"真", true },
	{ L"假", false },

};


bool is_num(wchar_t n) {

	if (n >= 48 && n <= 57) {
		return true;
	}
	else {
		return false;
	}
}

/*namespace pats {

	std::wregex num(L"^[0-9]+(\\.[0-9]+)?");

};*/

static std::map<token_type, token_symbol::type> new_symbol_map = {

	{ token_type::AT, token_symbol::type::AT },
	{ token_type::QUESTION, token_symbol::type::QUESTION },
	{ token_type::NOT, token_symbol::type::NOT },
	{ token_type::AND, token_symbol::type::AND },
	{ token_type::OR, token_symbol::type::OR },

	{ token_type::PLUS, token_symbol::type::PLUS },
	{ token_type::MINUS, token_symbol::type::MINUS },
	{ token_type::ASTERISK, token_symbol::type::ASTERISK },
	{ token_type::SLASH, token_symbol::type::SLASH },
	{ token_type::PERCENT, token_symbol::type::PERCENT },


	{ token_type::DOT, token_symbol::type::DOT },
	{ token_type::COMMA, token_symbol::type::COMMA },
	{ token_type::COLON, token_symbol::type::COLON },
	{ token_type::SEMICOLON, token_symbol::type::SEMICOLON },
	{ token_type::OPEN_PAREN, token_symbol::type::PAREN_LEFT },
	{ token_type::CLOSE_PAREN, token_symbol::type::PAREN_RIGHT },
	{ token_type::OPEN_BRACKET, token_symbol::type::BRACKET_LEFT },
	{ token_type::CLOSE_BRACKET, token_symbol::type::BRACKET_RIGHT },
	{ token_type::OPEN_BRACE, token_symbol::type::BRACE_LEFT },
	{ token_type::CLOSE_BRACE, token_symbol::type::BRACE_RIGHT },
	{ token_type::DOUBLE_ANGLE_BRACKET_LEFT, token_symbol::type::ANGLE_BRACKET_LEFT },
	{ token_type::DOUBLE_ANGLE_BRACKET_RIGHT, token_symbol::type::ANGLE_BRACKET_RIGHT },

	{ token_type::GREATER, token_symbol::type::GREATER },
	{ token_type::LESS, token_symbol::type::LESS },
	{ token_type::GREATER_EQUAL, token_symbol::type::GREATER_EQUAL },
	{ token_type::LESS_EQUAL, token_symbol::type::LESS_EQUAL },
	{ token_type::EQUAL_EQUAL, token_symbol::type::EQUAL_EQUAL },
	{ token_type::NOT_EQUAL, token_symbol::type::NOT_EQUAL },

	{ token_type::BELONG, token_symbol::type::SINGLE_ARROW },
	{ token_type::SUB, token_symbol::type::DOUBLE_ARROW },
	// { L"!=>", token_symbol::type::NOT_SUB },

	{ token_type::EQUAL, token_symbol::type::EQUAL },
	{ token_type::PLUS_EQUAL, token_symbol::type::PLUS_EQUAL },
	{ token_type::MINUS_EQUAL, token_symbol::type::MINUS_EQUAL },
	{ token_type::ASTERISK_EQUAL, token_symbol::type::ASTERISK_EQUAL },
	{ token_type::SLASH_EQUAL, token_symbol::type::SLASH_EQUAL },
	// { token_type::CARET_EQUAL, token_symbol::type::CARET_EQUAL },
	{ token_type::PERCENT_EQUAL, token_symbol::type::PERCENT_EQUAL },
};

trie<token_type> symbol_map = {

// 通用

	{ L"@", token_type::AT },
	{ L"?", token_type::QUESTION },
	{ L"!", token_type::NOT },
	{ L"&", token_type::AND },
	{ L"|", token_type::OR },
	{ L"\\", token_type::XOR },

	{ L"+", token_type::PLUS },
	{ L"-", token_type::MINUS },
	{ L"*", token_type::ASTERISK },
	{ L"/", token_type::SLASH },
	{ L"^", token_type::CARET },
	{ L"^/", token_type::CARET_SLASH },
	{ L"%", token_type::PERCENT },


	{ L".", token_type::DOT },
	{ L",", token_type::COMMA },
	{ L":", token_type::COLON },
	{ L";", token_type::SEMICOLON },
	{ L"(", token_type::OPEN_PAREN },
	{ L")", token_type::CLOSE_PAREN },
	{ L"[", token_type::OPEN_BRACKET },
	{ L"]", token_type::CLOSE_BRACKET },
	{ L"{", token_type::OPEN_BRACE },
	{ L"}", token_type::CLOSE_BRACE },
	{ L"<<", token_type::DOUBLE_ANGLE_BRACKET_LEFT },
	{ L">>", token_type::DOUBLE_ANGLE_BRACKET_RIGHT },

	{ L">", token_type::GREATER },
	{ L"<", token_type::LESS },
	{ L">=", token_type::GREATER_EQUAL },
	{ L"≥", token_type::GREATER_EQUAL },
	{ L"<=", token_type::LESS_EQUAL },
	{ L"≤", token_type::LESS_EQUAL },
	{ L"==", token_type::EQUAL_EQUAL },
	{ L"!=", token_type::NOT_EQUAL },
	{ L"≠", token_type::NOT_EQUAL },

	{ L"->", token_type::BELONG },
	{ L"=>", token_type::SUB },
	{ L"!->", token_type::NOT_BELONG },
	{ L"!=>", token_type::NOT_SUB },

	{ L"=", token_type::EQUAL },
	{ L"+=", token_type::PLUS_EQUAL },
	{ L"-=", token_type::MINUS_EQUAL },
	{ L"*=", token_type::ASTERISK_EQUAL },
	{ L"/=", token_type::SLASH_EQUAL },
	{ L"^=", token_type::CARET_EQUAL },
	{ L"^/=", token_type::CARET_SLASH_EQUAL },
	{ L"%=", token_type::PERCENT_EQUAL },

// zh-cn
	{ L"？", token_type::QUESTION },
	{ L"！", token_type::NOT },

	{ L"，", token_type::COMMA },
	{ L"：", token_type::COLON },
	{ L"；", token_type::SEMICOLON },
	{ L"（", token_type::OPEN_PAREN },
	{ L"）", token_type::CLOSE_PAREN },
	{ L"【", token_type::OPEN_BRACKET },
	{ L"】", token_type::CLOSE_BRACKET },
	{ L"｛", token_type::OPEN_BRACE },
	{ L"｝", token_type::CLOSE_BRACE },
	{ L"《", token_type::DOUBLE_ANGLE_BRACKET_LEFT },
	{ L"》", token_type::DOUBLE_ANGLE_BRACKET_RIGHT },

	{ L"！->", token_type::NOT_BELONG },
	{ L"！=>", token_type::NOT_SUB }, // replace_list

};

trie<null> ignore_set = {
	{ L" ", Null },
	{ L"\t", Null },
};

token_list scan(std::wstring filepath) {

	std::vector<token*> Token_list;
	std::wstring src = read(filepath, coding::UTF_8);

	Token_list.push_back(new token_symbol {token_type::BOF_, token_symbol::type::BOF_, 0, 0});

	int length = src.length();

	for (int i = 0; i < length; ) {

		int begin = i;

		// comments.sharp_comments
		if (src[i] == L'#') {

			do {
				i += 1;
				if (i >= length) {
					throw new unterminated_comments { filepath, position_format(src, i) };
				}
			} while (src[i] != L'#');
			i += 1;
		}

		// comments.single_line
		else if (src.substr(i, 2) == L";;") {
			i += 2;
			while (i < length && src[i] != L'\n') { //
				i += 1;
			}
		}

		// whitespace (ignored character)
		else if (auto _ = ignore_set.search(src, i)) { // `v
			// pass
		}

		// EOL
		else if (src[i] == L'\u000A' || src[i] == L'\u000D' || src[i] == L'\u0085') { // LF || CR || NEL
			if (Token_list.back()->TYPE != token_type::EOL_) {
				Token_list.push_back(new token_symbol {token_type::EOL_, token_symbol::type::EOL_, begin, i});
			}
			i += 1;
		}

		// escape
		else if (src[i] == L'`') {
			i += 1;
			if (src[i] == L'\n') {
				i += 1;
			}
			// else if '`x837' 汉字标识符
			else {
				throw new invalid_escape { filepath, position_format(src, i) };
			}
		}

		// string
		else if (string_head_matched(src[i])) {
			Token_list.push_back(lex_string(src, i, filepath));
		}

		// entity.literal.number
		else if (is_num(src[i])) {

/*			std::wsmatch regres;
			std::wstring matchfrom = src.substr(i);

			std::regex_search(matchfrom, regres, pats::num);
			std::wstring s_val = regres.str();*/
			int val = cvt_dec(src, i);

			Token_list.push_back(new token_number { val, begin, i });
		}

		// entity.identifier || keyword || literal
		else if (identifier_charset.include(src[i])) {

			std::wstring val;

			do {

				if (src[i] != L' ' || src[i-1] != L' ') {
					val += src[i];
				}
				i += 1;

			} while(i < length && identifier_charset.include(src[i]));

			// remove back spaces
			if (val[val.length()-1] == L' ') {
				val = val.substr(0, val.length()-1);
			}

			if (keyword_map.find(val) != keyword_map.end()) { // is keyword
				Token_list.push_back(new token_keyword { keyword_map[val], begin, i});
			}
			else if (literal_bool_map.find(val) != literal_bool_map.end()) {
				Token_list.push_back(new token_boolean { literal_bool_map[val], begin, i });
			}
			else { // is identifier
				Token_list.push_back(new token_identifier {val, begin, i});
			}

		}

		// token.symbol
		else if (auto val = symbol_map.search(src, i)) {
			Token_list.push_back(new token_symbol { *val, new_symbol_map[*val], begin, i });
		}

		else {
			throw new invalid_character { filepath, position_format(src, i), src[i] };
		}

	}

	Token_list.push_back(new token_symbol {token_type::EOF_, token_symbol::type::EOF_, length, length});
	return token_list { Token_list };

}
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

#include <stdexcept>

#include <locale>

#include "utils/ansi.hpp"

#include "exceptions/exception.hpp"
#include "exceptions/command_errors.hpp"


#include "scanner/scanner.hpp"
#include "exceptions/lex_errors.hpp"

#include "parser/parser.hpp"
#include "ast/node.hpp"

#include "semantic/class.hpp"
#include "semantic/environment.hpp"

#include "utils/ranges.hpp"
#include "utils/losh.hpp"

#include "extra/helper/helper.hpp"
#include "extra/packer/pack.hpp"

#include "runtime.hpp"

#include "utils/coding.hpp"

#include "model/number.hpp"
int main(int argc, char** args) {

    std::locale::global(std::locale(""));
    losh command = { argc, args };

    try {

        if (command.is(L"lex") || command.is(L"词法分析")) {
            std::wstring filepath;

            if (command.has_static()) {
                filepath = command.get_static();
            }
            else {
                throw new missing_argument { L"filepath" };
            }

            // bool color;
            std::wstring o = scan(filepath).view();
            std::wcout << rmansi(o) << std::endl;                
        
        }

        else if (command.is(L"parse")) {
            std::wstring filepath;

            if (command.has_static()) {
                filepath = command.get_static();
            }
            else {
                throw new missing_argument { L"filepath" };
            }
            
            node* ast = parse_exe(filepath);
            std::wstring o = ast->view();
            std::wcout << rmansi(o);
            
        }

        else if (command.is(L"run")) {
            std::wstring filepath;

            if (command.has_static()) {
                filepath = command.get_static();
            }
            else {
                throw new missing_argument { L"filepath" };
            }

            runtime Runtime;
            Runtime.filepath = filepath;
            Runtime.working_directory = std::filesystem::current_path();
            Runtime.debug.lang = language::en_us;

            statement_block* ast = parse_exe(filepath);

            auto env = environment {};
            ast->exec(env, Runtime);

            /*
                debug.lang=zh-cn
                paths=/path/to/dira&/path/to/dirb/*
            */

        }

        else if (command.is(L"test")) {
            test();
        }

        else if (command.is(L"pack")) {

        }
        
        else if (command.hasnt()) {
            // std::wcout << "introductions\n";
            help(command);
        }

        else {
            throw new unknown_command { command.get_static() };
        }
    }
    catch (exception* e) {
        std::wcerr << e->msg(language::en_us);
        return 1;
    }

    // temp
    catch (const char* e) {
        std::cerr << e << std::endl;
        return 1;
    }

    catch (std::wstring e) {
        std::wcerr << e << std::endl;
        return 1;
    }
    // std except

    return 0;
}
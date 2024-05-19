#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

#include <stdexcept>

#include <locale>

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

#include <gmpxx.h>
static void test() {
    // 初始化两个大数
    mpz_class a, b, c;

    // 赋值操作，使用字符串形式
    a = "123456789012345678901234567890"; // 非常大的数
    b = "987654321098765432109876543210";

    // 进行加法运算
    c = a + b;
    std::cout << "Sum: " << c << std::endl;

    // 进行乘法运算
    c = a * b;
    std::cout << "Product: " << c << std::endl;

    // 进行除法运算
    c = a / b;
    std::cout << "Quotient: " << c << std::endl;

    // 进行模运算
    c = a % b;
    std::cout << "Modulus: " << c << std::endl;

    // // 进行幂运算
    // c = a.powm(b, 10); // a 的 b 次幂模 10
    // std::cout << "Power Modulo: " << c << std::endl;

}

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
            std::wcout << o << std::endl;                
        
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
            std::wcout << o;
            
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

            auto ast = parse_exe(filepath);

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
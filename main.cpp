#include "utils/timeutil.h"
#include "utils/compilermanager.h"
#include "tokenizer/tokenizer.h"
#include "parser/parser.h"
#include <iostream>
#include <fstream>
#include <thread>

void run() {
    std::string code;
    {
        std::ifstream sourceFile("src/main.tl");
        if (!sourceFile) {
            auto path = std::filesystem::current_path().string() + "/src";
            std::cerr << "Source file not found in the " << path << " directory!" << std::endl;
            exit(-1);
        }
        std::string line;
        if (sourceFile.is_open()) {
            while (getline(sourceFile, line)) {
                bool commentFound = false;
                for (const char &character : line) {
                    if (character == ' ') {
                        continue;
                    }
                    if (character == '#') {
                        commentFound = true;
                        break;
                    }
                }
                if (!commentFound) {
                    code += line;
                }
            }
            sourceFile.close();
        }

    }
    turbolang::compilermanager::prepare();
    turbolang::compilermanager::output_to_file = true;
    class turbolang::tokenizer tokenizer;
    long start = turbolang::get_current_nano();
    std::vector<turbolang::token> tokens = tokenizer.tokenize(code);

    long end = turbolang::get_current_nano();
    for (const auto &t : tokens) {
        //t.debug();
    }
    std::cout << "TurboLang took " << (end - start)
                 << " nanoseconds to tokenize the source code!" << std::endl;
    start = turbolang::get_current_nano();
    class turbolang::parser parser;
    parser.parse(tokens);
    end = turbolang::get_current_nano();
    std::cout << "TurboLang took " << (end - start)
              << " nanoseconds to parse the tokens!" << std::endl;
    start = turbolang::get_current_nano();
    turbolang::compilermanager::generate_byte_code(0);
    turbolang::compilermanager::cleanup_byte_code(parser);
    end = turbolang::get_current_nano();
    std::cout << "TurboLang took " << (end - start) << " nanoseconds to generate bytecode!"
              << std::endl;
    start = turbolang::get_current_nano();
    turbolang::compilermanager::generate_binary();
    end = turbolang::get_current_nano();
    std::cout << "Clang took " << (end - start)
              << " nanoseconds to compile the bytecode(convert into machine code)." << std::endl;
    std::cout << "Executing in 5 seconds..." << std::endl;
    auto sleepTime = std::chrono::seconds(5);
    std::this_thread::sleep_for(sleepTime);
    std::cout << std::endl;
    turbolang::compilermanager::execute_binary();
    std::cout << std::endl;
    std::cout << std::endl;
}

int main() {
    run();
}
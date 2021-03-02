#include "utils/timeutil.h"
#include "utils/compilermanager.h"
#include "utils/printer.h"
#include "tokenizer/tokenizer.h"
#include "parser/parser.h"
#include <iostream>

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
                code += line;
            }
            sourceFile.close();
        }

    }
    turbolang::compilermanager::prepare();
    turbolang::compilermanager::output_to_file = true;
    class turbolang::tokenizer tokenizer;
    std::vector<turbolang::token> tokens = tokenizer.tokenize(code);
    /*for (const auto& t : tokens) {
        t.debug();
    }*/
    class turbolang::parser parser;
    parser.parse(tokens);
    std::cout << "Parsing source code..." << std::endl;
    std::cout << "Finished parsing source code!" << std::endl;
    std::cout << "Generating bytecode..." << std::endl;
    turbolang::compilermanager::generate_byte_code(0);
    std::cout << "Generated bytecode!" << std::endl;
    std::cout << "Generating binary..." << std::endl;
    turbolang::compilermanager::generate_binary();
    std::cout << "Generated binary!" << std::endl;
    std::cout << "Executing binary..." << std::endl;
    turbolang::compilermanager::execute_binary();
    std::cout << "Finished executing the binary!" << std::endl;
}

int main() {
    long startTime = turbolang::get_current_nano();
    run();
    long endTime = turbolang::get_current_nano() - startTime;
    std::cout << "Program took " << endTime << "ns to execute." << std::endl;
}


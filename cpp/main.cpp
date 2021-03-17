#include "utils/timeutil.h"
#include "utils/Compiler.h"
#include "utils/llvm/LLVMManager.h"
#include "tokenizer/tokenizer.h"
#include "parser/Parser.h"
#include "utils/SourceCodeReader.h"
#include <iostream>
using namespace turbolang;
void run() {
    std::string code;
    std::ifstream sourceFile("build/src/main.tl");
    //The filtered source code will be inserted into the variable we passed in as a pointer.
    SourceCodeReader::readAndFilterCode(sourceFile, &code);
    LLVMManager::init();
    class turbolang::tokenizer tokenizer;
    long start = turbolang::get_current_nano();
    std::vector<turbolang::token> tokens = tokenizer.tokenize(code);
    long end = turbolang::get_current_nano();
    for (const auto &t : tokens) {
        t.debug();
    }
    std::cout << "TurboLang took " << (end - start)
              << " nanoseconds to tokenize the source code!" << std::endl;
    start = turbolang::get_current_nano();
    class turbolang::Parser parser;
    parser.parse(tokens);
    end = get_current_nano();
    std::cout << "TurboLang took " << (end - start)
              << " nanoseconds to parse the tokens!" << std::endl;
    start = get_current_nano();
    Compiler::generate_byte_code();
    end = get_current_nano();
    std::cout << "TurboLang took " << (end - start) << " nanoseconds to generate bytecode!"
              << std::endl;
    start = get_current_nano();
    Compiler::generate_binary();
    end = get_current_nano();
    std::cout << "Clang took " << (end - start)
              << " nanoseconds to compile the bytecode(convert into machine code)." << std::endl;
    start = get_current_nano();
    Compiler::generate_executables();
    end = get_current_nano();
    std::cout << "Clang took " << (end - start) << " nanoseconds to build the executables for all platforms."
              << std::endl;
    LLVMManager::destroy();
    std::cout << "Executing in 5 seconds..." << std::endl;
    auto sleepTime = std::chrono::seconds(5);
    std::this_thread::sleep_for(sleepTime);
    std::cout << std::endl;
    Compiler::execute_binary();
    std::cout << std::endl;
    std::cout << std::endl;
}

int main() {
    run();
}
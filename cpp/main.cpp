#include "utils/TimeUtil.h"
#include "utils/Compiler.h"
#include "utils/llvm/LLVMManager.h"
#include "tokenizer/Tokenizer.h"
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
    class Tokenizer tokenizer;
    long start = turbolang::getCurrentNanoTime();
    std::vector<Token> tokens = tokenizer.tokenize(code);
    long end = turbolang::getCurrentNanoTime();
    for (const auto &t : tokens) {
        t.debug();
    }
    std::cout << "TurboLang took " << (end - start)
              << " nanoseconds to tokenize the source code!" << std::endl;
    start = turbolang::getCurrentNanoTime();
    class turbolang::Parser parser;
    parser.parse(tokens);
    end = getCurrentNanoTime();
    std::cout << "TurboLang took " << (end - start)
              << " nanoseconds to parse the tokens!" << std::endl;
    start = getCurrentNanoTime();
    Compiler::generate_byte_code();
    end = getCurrentNanoTime();
    std::cout << "TurboLang took " << (end - start) << " nanoseconds to generate bytecode!"
              << std::endl;
    start = getCurrentNanoTime();
    Compiler::generate_binary();
    end = getCurrentNanoTime();
    std::cout << "Clang took " << (end - start)
              << " nanoseconds to compile the bytecode(convert into machine code)." << std::endl;
    start = getCurrentNanoTime();
    Compiler::generate_executables();
    end = getCurrentNanoTime();
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
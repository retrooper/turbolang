#include "utils/TimeUtil.h"
#include "utils/Compiler.h"
#include "utils/llvm/LLVMManager.h"
#include "token/Tokenizer.h"
#include "builder/Builder.h"
#include "parser/Parser.h"
#include "utils/SourceCodeReader.h"
#include "utils/Logger.h"
#include <iostream>
using namespace turbolang;
void run() {
    std::string code;
    std::ifstream sourceFile("build/src/main.tl");
    //The filtered source code will be inserted into the variable we passed in as a pointer.
    if (SourceCodeReader::readAndFilterCode(sourceFile, &code) == SOURCE_READER_RESULT_FAILURE) {
        throw std::runtime_error("Failed to read and filter source code!");
    }
    Logger::mode = LOG_MODE_WARNING;
    LLVMManager::init();
    long start = turbolang::getCurrentNanoTime();
    std::vector<Token> tokens = Tokenizer::tokenize(code);
    long end = turbolang::getCurrentNanoTime();
    for (const auto &t : tokens) {
        t.debug();
    }
    LOG_WARN("TurboLang took " << (end - start) << " nanoseconds to tokenize the source code!");
    start = turbolang::getCurrentNanoTime();
    Parser::parse(tokens);
    end = getCurrentNanoTime();
    LOG_WARN("TurboLang took " << (end - start) << " nanoseconds to parse the tokens!");
    start = getCurrentNanoTime();
    Compiler::generateBytecode();
    LLVMManager::destroy();
    end = getCurrentNanoTime();
    LOG_WARN("TurboLang took " << (end - start) << " nanoseconds to generate bytecode!");
    start = getCurrentNanoTime();
    Builder::buildExecutables();
    end = getCurrentNanoTime();
    LOG_WARN("Clang took " << (end - start) << " nanoseconds to build the executables for all platforms.");
    LOG_WARN("Executing in 5 seconds...");
    auto sleepTime = std::chrono::seconds(5);
    std::this_thread::sleep_for(sleepTime);
    LOG_WARN("");
    //Execute the generated binary
    std::system("./binary/output.out");
}

int main() {
    run();
}
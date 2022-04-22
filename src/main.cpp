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
    //Get out of cmake-build-debug folder
    std::filesystem::current_path("../");

    std::ifstream sourceFile("samples/main.tl");
    //The filtered source code will be inserted into the variable we passed in as a pointer.
    if (SourceCodeReader::readAndFilterCode(sourceFile, &code) == SOURCE_READER_RESULT_FAILURE) {
        throw std::runtime_error("Failed to read and filter source code!");
    }
    std::cout << "code: \n" << code << std::endl;
    Logger::mode = LOG_MODE_WARNING;
    LLVMManager::init();
    long start = turbolang::getCurrentMicroTime();
    std::vector<Token> tokens = Tokenizer::tokenize(code);
    long end = turbolang::getCurrentMicroTime();
    for (const auto &t : tokens) {
        t.debug();
    }
    LOG_WARN("TurboLang took " << (end - start) << " microseconds to tokenize the source code!");
    start = turbolang::getCurrentMicroTime();
    Parser::parse(tokens);
    end = getCurrentMicroTime();
    LOG_WARN("TurboLang took " << (end - start) << " microseconds to parse the tokens!");
    start = getCurrentMicroTime();
    Compiler::generateBytecode();
    LLVMManager::destroy();
    end = getCurrentNanoTime();
    LOG_WARN("TurboLang took " << (end - start) << " microseconds to generate bytecode!");
    start = getCurrentMicroTime();
    Builder::buildExecutables();
    end = getCurrentMicroTime();
    LOG_WARN("Clang took " << (end - start) << " microseconds to build the executables for all platforms.");
    LOG_WARN("Executing in 5 seconds...");
    LOG_WARN("-");
    auto sleepTime = std::chrono::seconds(5);
    std::this_thread::sleep_for(sleepTime);
    //Execute the generated binary
    std::system("./binary/output.out");
}

int main() {
    run();
}
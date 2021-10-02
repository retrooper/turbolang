#include "utils/Compiler.h"

void turbolang::Compiler::generateBytecode() {
    llvm::raw_fd_ostream *outputStream = &llvm::outs();
    auto current_path = std::filesystem::current_path().string() + "/build";
    std::filesystem::current_path(current_path);
    std::string new_path = current_path + "/bytecode";
    if (!std::filesystem::exists(new_path)) {
        std::filesystem::create_directory(new_path);
    }
    std::filesystem::current_path(new_path);
    const char *ouputFileName = "output.ll";
    std::error_code EC;
    outputStream = new llvm::raw_fd_ostream(ouputFileName, EC);
    if (EC.message() != "Success") {
        LOG_ERROR("LLVM Compilation error code: " << EC.message());
    }
    LLVMManager::llvmModule->print(*outputStream, nullptr);
    if (outputStream != &llvm::outs()) {
        delete outputStream;
    }
}
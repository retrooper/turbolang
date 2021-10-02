#include "utils/Compiler.h"

void turbolang::Compiler::generateBytecode() {
    llvm::raw_fd_ostream *outputStream = &llvm::outs();
    auto currentPath = std::filesystem::current_path().string() + "/build";
    //Does build folder exist?
    if (!std::filesystem::exists(currentPath)) {
        //Create it if it doesn't exist
        std::filesystem::create_directories(currentPath);
    }
    std::filesystem::current_path(currentPath);
    std::string new_path = currentPath + "/bytecode";
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
#include "utils/llvm/LLVMManager.h"

namespace turbolang {
    llvm::LLVMContext* LLVMManager::llvmCtx = nullptr;
    llvm::IRBuilder<>* LLVMManager::llvmBytecodeBuilder = nullptr;
    llvm::Module* LLVMManager::llvmModule = nullptr;
    void LLVMManager::init() {
        llvmCtx = new llvm::LLVMContext();
        llvmBytecodeBuilder = new llvm::IRBuilder<>(*llvmCtx);
        llvmModule = new llvm::Module("inputFile", *llvmCtx);
    }

    void LLVMManager::destroy() {
        delete llvmModule;
        delete llvmBytecodeBuilder;
        delete llvmCtx;
    }
}
#pragma once
#include "LLVMIncluder.h"
namespace turbolang {
    class LLVMManager {
    public:
        static llvm::LLVMContext* llvmCtx;
        static llvm::Module* llvmModule;
        static llvm::IRBuilder<>* llvmBytecodeBuilder;

        static void init();

        static void destroy();
    };
}


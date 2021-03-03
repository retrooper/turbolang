#pragma once

#include "llvm/ADT/APInt.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include <string>

namespace turbolang {
    class compilermanager {
        friend class printer;
        friend class sleeper;
        friend class exiter;
        friend class parser;
    private:
        static llvm::LLVMContext llvmContext;
        static llvm::IRBuilder<> llvmIRBuilder;
        static std::unique_ptr<llvm::Module> llvmModule;
        static llvm::FunctionType *llvmMainFunctionType;
        static llvm::Function *llvmMainFunction;

    public:
        static bool output_to_file;
        static void prepare();
        static void generate_byte_code(int exitCode);
        static void generate_binary();
        static void execute_binary();
    };
}
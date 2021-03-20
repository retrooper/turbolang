#pragma once

#include "utils/llvm/LLVMManager.h"
#include <vector>

namespace turbolang {
    class FunctionInternal {
    public:
        static void defineFunction(const std::string &functionName, llvm::Type *returnType,
                                   std::vector<llvm::Type *> &argumentTypes, bool argumentSizeConstant);

        static void defineFunction(const std::string &functionName, llvm::Type *returnType, llvm::Type *argumentType,
                                   bool argumentSizeConstant);

        static llvm::Value *callFunction(const std::string &functionName, std::vector<llvm::Value *> arguments);

        static llvm::Value *callFunction(const std::string &functionName, llvm::Value *argument);
        virtual void load();

        virtual llvm::Value *execute(std::vector<llvm::Value *> &arguments);
    };
}
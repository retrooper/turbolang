#include "function/FunctionCallProcessor.h"

namespace turbolang {
    llvm::Value *
    turbolang::FunctionCallProcessor::process(std::string functionName, std::vector<llvm::Value *> &arguments) {
        llvm::Function* llvmFunction = LLVMManager::llvmModule->getFunction(functionName);
        return LLVMManager::llvmBytecodeBuilder->CreateCall(llvmFunction, arguments);
    }
}

#include "function/FunctionCallProcessor.h"

namespace turbolang {
    llvm::Value *
    turbolang::FunctionCallProcessor::process(std::string functionName, std::vector<llvm::Value *> &arguments) {
        llvm::Function* llvmFunction = LLVMManager::llvmModule->getFunction(functionName);
        if (llvmFunction == nullptr) {
            std::string msg = "Failed to find function: " + functionName;
            std::cerr << msg << std::endl;
            std::exit(-1);
        }
        return LLVMManager::llvmBytecodeBuilder->CreateCall(llvmFunction, arguments);
    }
}

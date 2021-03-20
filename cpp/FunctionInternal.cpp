#include "function/internal/FunctionInternal.h"

namespace turbolang {
    void FunctionInternal::load() {

    }

    llvm::Value *FunctionInternal::execute(std::vector<llvm::Value *> &arguments) {
        return nullptr;
    }

    void FunctionInternal::defineFunction(const std::string &functionName, llvm::Type *returnType,
                                          std::vector<llvm::Type *> &argumentTypes, bool argumentSizeConstant) {
        llvm::FunctionType *functionType =
                llvm::FunctionType::get(returnType, argumentTypes, !argumentSizeConstant);
        llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, functionName, LLVMManager::llvmModule);

    }

    void FunctionInternal::defineFunction(const std::string &functionName, llvm::Type *returnType, llvm::Type *argumentType,
                                          bool argumentSizeConstant) {
        llvm::FunctionType *functionType =
                llvm::FunctionType::get(returnType, argumentType, !argumentSizeConstant);
        llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, functionName, LLVMManager::llvmModule);
    }

    llvm::Value* FunctionInternal::callFunction(const std::string &functionName, std::vector<llvm::Value *> arguments) {
        llvm::Function* llvmFunction = LLVMManager::llvmModule->getFunction(functionName);
        return LLVMManager::llvmBytecodeBuilder->CreateCall(llvmFunction, arguments);
    }

    llvm::Value* FunctionInternal::callFunction(const std::string &functionName, llvm::Value *argument) {
        llvm::Function* llvmFunction = LLVMManager::llvmModule->getFunction(functionName);
        return LLVMManager::llvmBytecodeBuilder->CreateCall(llvmFunction, argument);
    }
};
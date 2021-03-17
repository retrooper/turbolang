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

    /**
     * INTERNAL FUNCTION DEFINITIONS:
     */

    /*
     * i32 printf(i8*, ...)
     */
    void PrintfFunctionInternal::load() {
        defineFunction("printf", LLVMManager::llvmBytecodeBuilder->getInt32Ty(), LLVMManager::llvmBytecodeBuilder->getInt8PtrTy(), false);
    }

    llvm::Value *PrintfFunctionInternal::execute(std::vector<llvm::Value *> &arguments) {
        return callFunction("printf", arguments);
    }

    /**
     * i32 puts(i8*)
     */
    void PrintlnFunctionInternal::load() {
        defineFunction("puts", LLVMManager::llvmBytecodeBuilder->getInt32Ty(), LLVMManager::llvmBytecodeBuilder->getInt8PtrTy(), true);
    }

    llvm::Value *PrintlnFunctionInternal::execute(std::vector<llvm::Value *> &arguments) {
        return callFunction("puts", arguments);
    }

    /**
     * void exit(i32)
     */
    void ExitFunctionInternal::load() {
        defineFunction("exit", LLVMManager::llvmBytecodeBuilder->getVoidTy(), LLVMManager::llvmBytecodeBuilder->getInt32Ty(), true);
    }

    llvm::Value *ExitFunctionInternal::execute(std::vector<llvm::Value *> &arguments) {
        return callFunction("exit", arguments);
    }

    /**
     * int usleep(u32)
     */
    void SleepFunctionInternal::load() {
        defineFunction("usleep", LLVMManager::llvmBytecodeBuilder->getInt32Ty(), LLVMManager::llvmBytecodeBuilder->getInt32Ty(), true);
    }

    llvm::Value *SleepFunctionInternal::execute(std::vector<llvm::Value *> &arguments) {
        return callFunction("usleep", arguments);
    }

    /**
     * int system(i8*)
     */
    void SystemFunctionInternal::load() {
        defineFunction("system", LLVMManager::llvmBytecodeBuilder->getInt32Ty(), LLVMManager::llvmBytecodeBuilder->getInt8PtrTy(), true);
    }

    llvm::Value *SystemFunctionInternal::execute(std::vector<llvm::Value *> &arguments) {
        return callFunction("system", arguments);
    }

    /**
     * i32 scanf(i8*, ...)
     */
    void ScanfFunctionInternal::load() {
        defineFunction("scanf", LLVMManager::llvmBytecodeBuilder->getInt32Ty(), LLVMManager::llvmBytecodeBuilder->getInt8PtrTy(), false);
    }

    llvm::Value *ScanfFunctionInternal::execute(std::vector<llvm::Value *> &arguments) {
        return callFunction("scanf", arguments);
    }
};
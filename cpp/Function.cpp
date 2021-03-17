#include "function/Function.h"

namespace turbolang {
    std::map<std::string, Function> Function::functionMap;
    Function::Function(const std::string &name) {
        this->name = name;
        this->type = FUNCTION_TYPE_VOID;
    }

    Function::Function(const std::string &name, FunctionType type) {
        this->name = name;
        this->type = type;
    }

    void Function::create(turbolang::FunctionType returnValue) {
        llvm::FunctionType *llvmFunctionType;
        if (arguments.empty()) {
            llvmFunctionType = llvm::FunctionType::get(Type::getLLVMTypeByFunctionType(returnValue), false);
        } else {
            std::vector<llvm::Type *> params;
            for (const FunctionArgument &argument : arguments) {
                params.push_back(Type::getLLVMTypeByVariableType(argument.type));
            }
            llvmFunctionType = llvm::FunctionType::get(Type::getLLVMTypeByFunctionType(returnValue), params, false);
        }
        llvmFunction = llvm::Function::Create(llvmFunctionType, llvm::Function::ExternalLinkage, name, LLVMManager::llvmModule);
        entry = llvm::BasicBlock::Create(*LLVMManager::llvmCtx, "entry", llvmFunction);
        for (int i = 0; i < llvmFunction->arg_size(); i++) {
            const FunctionArgument *argument = &arguments[i];
            llvm::AllocaInst *allocaInst = LLVMManager::llvmBytecodeBuilder->CreateAlloca(
                    Type::getLLVMTypeByVariableType(argument->type), nullptr, argument->name);
            allocaMap[name] = allocaInst;
        }
    }

    void Function::create(FunctionType returnValue, std::vector<FunctionArgument> &inArguments) {
        arguments = inArguments;
        create(returnValue);
    }

    //Pointer of a variable
    llvm::AllocaInst *Function::getAllocaInst(const std::string &name) {
        return allocaMap[name];
    }

    void Function::setAllocaInst(const std::string& name, llvm::AllocaInst* allocaInst) {
        allocaMap[name] = allocaInst;
    }

    //Get variable value
    llvm::Value *Function::getValue(const std::string &name) {
        llvm::AllocaInst *allocaInst = getAllocaInst(name);
        llvm::LoadInst *loadInst = LLVMManager::llvmBytecodeBuilder->CreateLoad(allocaInst, name);
        return loadInst;
    }

    void Function::setValue(const std::string &name, llvm::Value *value) {
        //Local variables(inside the current function)
        llvm::AllocaInst *allocaInst = getAllocaInst(name);
        llvm::StoreInst *storeInst = LLVMManager::llvmBytecodeBuilder->CreateStore(value, allocaInst, false);
    }
}
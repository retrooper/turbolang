#include "function/Function.h"

namespace turbolang {
    std::map<std::string, Function> Function::functionMap;

    Function::Function(const std::string &name, DataType type) {
        this->name = name;
        this->type = type;
    }

    void Function::create() {
        llvm::FunctionType *llvmFunctionType;
        if (arguments.empty()) {
            llvmFunctionType = llvm::FunctionType::get(Type::getLLVMType(type), false);
        } else {
            std::vector<llvm::Type *> params;
            for (const FunctionArgument &argument : arguments) {
                params.push_back(Type::getLLVMType(argument.type));
            }
            llvmFunctionType = llvm::FunctionType::get(Type::getLLVMType(type), params, false);
        }
        llvmFunction = llvm::Function::Create(llvmFunctionType, llvm::Function::ExternalLinkage, name, LLVMManager::llvmModule);
        entry = llvm::BasicBlock::Create(*LLVMManager::llvmCtx, "entry", llvmFunction);
        LLVMManager::llvmBytecodeBuilder->SetInsertPoint(entry);
        for (int i = 0; i < llvmFunction->arg_size(); i++) {
            const FunctionArgument *argument = &arguments[i];
            llvm::AllocaInst *allocaInst = LLVMManager::llvmBytecodeBuilder->CreateAlloca(Type::getLLVMType(argument->type), nullptr,
                                                                                          llvm::Twine(argument->name));
            llvm::StoreInst* storeInst = LLVMManager::llvmBytecodeBuilder->CreateStore(llvmFunction->getArg(i), allocaInst, false);
            setAllocaInst(argument->name, allocaInst);
        }
    }

    void Function::create(std::vector<FunctionArgument> &inArguments) {
        arguments = inArguments;
        create();
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
        llvm::AllocaInst *allocaInst = getAllocaInst(name);
        llvm::StoreInst *storeInst = LLVMManager::llvmBytecodeBuilder->CreateStore(value, allocaInst, false);
    }
}
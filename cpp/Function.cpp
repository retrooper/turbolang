#include "function/Function.h"

namespace turbolang {
    std::map<std::string, Function> Function::functionMap;

    Function::Function(const std::string &name, DataType type, const std::string &extraData) {
        this->name = name;
        this->type = type;
        this->extraData = extraData;
    }

    void Function::create() {
        llvm::FunctionType *llvmFunctionType;
        if (arguments.empty()) {
            llvmFunctionType = llvm::FunctionType::get(Type::getLLVMType(type, extraData), false);
        } else {
            std::vector<llvm::Type *> params;
            for (const FunctionArgument &argument : arguments) {
                params.push_back(Type::getLLVMType(argument.type));
            }
            llvmFunctionType = llvm::FunctionType::get(Type::getLLVMType(type), params, false);
        }
        llvmFunction = llvm::Function::Create(llvmFunctionType, llvm::Function::ExternalLinkage, name,
                                              LLVMManager::llvmModule);

        entry = llvm::BasicBlock::Create(*LLVMManager::llvmCtx, "entry", llvmFunction);
        LLVMManager::llvmBytecodeBuilder->SetInsertPoint(entry);
        for (int i = 0; i < llvmFunction->arg_size(); i++) {
            const FunctionArgument *argument = &arguments[i];
            llvm::AllocaInst *allocaInst = LLVMManager::llvmBytecodeBuilder->CreateAlloca(
                    Type::getLLVMType(argument->type), nullptr,
                    llvm::Twine(argument->name));
            llvm::StoreInst *storeInst = LLVMManager::llvmBytecodeBuilder->CreateStore(llvmFunction->getArg(i),
                                                                                       allocaInst, false);
            setAllocaInst(argument->name, allocaInst);
        }
    }

    //Pointer of a variable
    llvm::AllocaInst *Function::getAllocaInst(std::string name) {
        std::vector<std::string> tokens = split(name, ".");
        if (tokens.size() == 1) {
            return allocaMap[name];
        } else {
            //Struct type alloca
            llvm::AllocaInst *allocaInst = getAllocaInst(tokens[0]);
            llvm::StructType *structType = (llvm::StructType *) allocaInst->getAllocatedType();
            Class clazz = Class::classMap[structType->getName().str()];
            unsigned int memberIndex = clazz.getMemberIndex(tokens[1]);
            return (llvm::AllocaInst *) LLVMManager::llvmBytecodeBuilder->CreateStructGEP(allocaInst, memberIndex,
                                                                                          llvm::Twine(tokens[1]));
        }
    }

    void Function::setAllocaInst(const std::string &name, llvm::AllocaInst *allocaInst) {
        allocaMap[name] = allocaInst;
    }

    //Get variable value
    llvm::Value *Function::getValue(std::string name) {
        llvm::AllocaInst *allocaInst = getAllocaInst(name);
        llvm::LoadInst *loadInst = LLVMManager::llvmBytecodeBuilder->CreateLoad(allocaInst, name);
        return loadInst;
    }

    void Function::setValue(std::string name, llvm::Value *value) {
        llvm::AllocaInst *allocaInst = getAllocaInst(name);
        llvm::StoreInst *storeInst = LLVMManager::llvmBytecodeBuilder->CreateStore(value, allocaInst, false);
    }

    std::vector<std::string> Function::split(std::string str, std::string delim) {
        std::vector<std::string> tokens;
        if (str.find(delim) == std::string::npos) {
            tokens.push_back(str);
            return tokens;
        }
        size_t prev = 0, pos = 0;
        do {
            pos = str.find(delim, prev);
            if (pos == std::string::npos) pos = str.length();
            std::string token = str.substr(prev, pos - prev);
            if (!token.empty()) tokens.push_back(token);
            prev = pos + delim.length();
        } while (pos < str.length() && prev < str.length());
        return tokens;
    }
}
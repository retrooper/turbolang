#include "function/Function.h"

#include <utility>

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
                llvm::Type *argType = Type::getLLVMType(argument.type, argument.typeInfo);
                if (argument.isPtr) {
                    argType = argType->getPointerTo();
                }
                params.push_back(argType);
            }
            llvmFunctionType = llvm::FunctionType::get(Type::getLLVMType(type), params, false);
        }
        llvmFunction = llvm::Function::Create(llvmFunctionType, llvm::Function::ExternalLinkage, name,
                                              LLVMManager::llvmModule);

        entry = llvm::BasicBlock::Create(*LLVMManager::llvmCtx, "entry", llvmFunction);
        LLVMManager::llvmBytecodeBuilder->SetInsertPoint(entry);
        for (int i = 0; i < llvmFunction->arg_size(); i++) {
            const FunctionArgument *argument = &arguments[i];
            llvm::Type *argType = Type::getLLVMType(argument->type, argument->typeInfo);
            if (argument->isPtr) {
                argType = argType->getPointerTo();
            }
            llvm::AllocaInst *allocaInst = LLVMManager::llvmBytecodeBuilder->CreateAlloca(
                    argType, nullptr,
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
            llvm::AllocaInst *allocaInst = allocaMap[tokens[0]];
            auto *structType = (llvm::StructType *) allocaInst->getAllocatedType();
            Class clazz = Class::classMap[structType->getName().str()];
            unsigned int memberIndex = clazz.getMemberIndex(tokens[1]);
            return (llvm::AllocaInst *) LLVMManager::llvmBytecodeBuilder->CreateStructGEP(allocaInst, memberIndex,
                                                                                          llvm::Twine(tokens[1]));
        }
    }

    llvm::Type *Function::getType(std::string name) {
        return getValue(std::move(name))->getType();
    }

    void Function::setAllocaInst(const std::string &name, llvm::AllocaInst *allocaInst) {
        allocaMap[name] = allocaInst;
    }

    //Get variable value
    llvm::Value *Function::getValue(std::string name) {
        bool pointerValue = name.find('&') == 0;
        llvm::AllocaInst *allocaInst = getAllocaInst(pointerValue ? name.substr(1) : name);
        if (pointerValue) {
            return allocaInst;
        } else {
            return LLVMManager::llvmBytecodeBuilder->CreateLoad(allocaInst, name.substr(1));
        }
    }

    llvm::Value *Function::getDereferencedValue(std::string name) {
        std::vector<std::string> tokens = split(name, ".");
        if (tokens.size() == 1) {
            llvm::Value* allocaInst = getValue(name);
            return LLVMManager::llvmBytecodeBuilder->CreateLoad(allocaInst);
        } else {
            //Struct type alloca
            llvm::AllocaInst *allocaInst = allocaMap[tokens[0]];
            llvm::Value* instance = LLVMManager::llvmBytecodeBuilder->CreateLoad(allocaInst);
            auto *structType = (llvm::StructType *) allocaInst->getAllocatedType();
            llvm::outs() << "Class name: " << structType->getName().str() << "\n";
            Class clazz = Class::classMap[structType->getName().str()];
            unsigned int memberIndex = clazz.getMemberIndex(tokens[1]);
            llvm::AllocaInst* ptr =  (llvm::AllocaInst *) LLVMManager::llvmBytecodeBuilder->CreateStructGEP(instance, memberIndex,
                                                                                          llvm::Twine(tokens[1]));
            return LLVMManager::llvmBytecodeBuilder->CreateLoad(ptr);
        }
    }

    void Function::setValue(std::string name, llvm::Value *value) {
        llvm::AllocaInst *allocaInst = getAllocaInst(std::move(name));
        llvm::StoreInst *storeInst = LLVMManager::llvmBytecodeBuilder->CreateStore(value, allocaInst);
    }

    void Function::setDereferencedValue(std::string name, llvm::Value *value) {
        llvm::Value *pointer = getValue(std::move(name));
        llvm::StoreInst *storeInst = LLVMManager::llvmBytecodeBuilder->CreateStore(value, pointer);
    }

    std::vector<std::string> Function::split(const std::string &str, const std::string &delim) {
        std::vector<std::string> tokens;
        if (str.find(delim) == std::string::npos) {
            tokens.push_back(str);
            return tokens;
        }
        size_t prev = 0, pos;
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
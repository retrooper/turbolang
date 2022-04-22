#include "function/Function.h"

#include <utility>

namespace turbolang {
    std::vector<Function> Function::functions;

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
            for (const FunctionArgument &argument: arguments) {
                llvm::Type *argType = Type::getLLVMType(argument.type, argument.typeInfo);
                if (argument.isPtr) {
                    argType = argType->getPointerTo();
                }
                params.push_back(argType);
            }
            llvmFunctionType = llvm::FunctionType::get(Type::getLLVMType(type, extraData), params, false);
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
            std::cout << "name: " << name << std::endl;
            std::cout << "clazz: " << clazz.name << std::endl;
            unsigned int memberIndex = clazz.getMemberIndex(tokens[1]);
            llvm::AllocaInst * inst = (llvm::AllocaInst *)((llvm::AllocaInst *)
                    LLVMManager::llvmBytecodeBuilder->CreateStructGEP(structType,
                                                                      allocaInst, memberIndex,
                                                                      llvm::Twine(tokens[1])));
            llvm::outs() << "Inst: " << *inst->getType() << "\n";
            return inst;
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
            if (allocaInst == nullptr) {
                return nullptr;
            }
            llvm::outs() << "value of type: " << *allocaInst->getAllocatedType() << ", vs alloca type: " << *allocaInst->getType() << "\n";
            return
                    LLVMManager::llvmBytecodeBuilder->CreateLoad(allocaInst->getAllocatedType(),
                                                                 allocaInst, name.substr(1));
        }
    }

    llvm::Value *Function::getDereferencedValue(std::string name) {
        std::vector<std::string> tokens = split(name, ".");
        if (tokens.size() == 1) {
            llvm::Value *allocaInst = getValue(name);
            return LLVMManager::llvmBytecodeBuilder->CreateLoad(allocaInst->getType(),
                                                                allocaInst, name.substr(1));
        } else {
            //Struct type alloca
            llvm::AllocaInst *allocaInst = allocaMap[tokens[0]];
            llvm::Value *instance = LLVMManager::llvmBytecodeBuilder->CreateLoad(allocaInst->getAllocatedType(),
                                                                                 allocaInst);
            auto *structType = (llvm::StructType *) allocaInst->getAllocatedType();
            Class clazz = Class::classMap[structType->getName().str()];
            unsigned int memberIndex = clazz.getMemberIndex(tokens[1]);
            llvm::AllocaInst *ptr = (llvm::AllocaInst *) LLVMManager::llvmBytecodeBuilder->CreateStructGEP(
                    instance->getType(),
                    instance,
                    memberIndex,
                    llvm::Twine(
                            tokens[1]));
            return LLVMManager::llvmBytecodeBuilder->CreateLoad(ptr->getAllocatedType(), ptr);
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

    Function *Function::getFunction(const std::string &functionName, const DataType &returnType,
                                    const std::vector<DataType> &argumentTypes) {
        for (int i = 0; i < functions.size(); i++) {
            Function *function = &functions[i];
            if (function->name == functionName
                && function->type == returnType
                && function->arguments.size() == argumentTypes.size()) {
                bool allMatching = true;
                for (const auto &funcArg: function->arguments) {
                    if (funcArg.type != argumentTypes[i]) {
                        allMatching = false;
                        break;
                    }
                }
                if (allMatching) {
                    return function;
                }
            }
        }
        return nullptr;
    }

    std::vector<Function *> Function::getFunctions(const std::string &functionName,
                                                   const std::vector<DataType> &argumentTypes) {
        std::vector<Function *> validFunctions;
        for (int i = 0; i < functions.size(); i++) {
            Function *function = &functions[i];
            if (function->name == functionName
                && function->arguments.size() == argumentTypes.size()) {
                bool allMatching = true;
                for (const auto &funcArg: function->arguments) {
                    if (funcArg.type != argumentTypes[i]) {
                        allMatching = false;
                        break;
                    }
                }
                if (allMatching) {
                    validFunctions.push_back(function);
                }
            }
        }
        return validFunctions;
    }

    std::vector<Function *> Function::getFunctions(const std::string &functionName) {
        std::vector<Function *> validFunctions;
        for (auto &i: functions) {
            Function *function = &i;
            if (function->name == functionName) {
                validFunctions.push_back(function);
            }
        }
        return validFunctions;
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
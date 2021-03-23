#include "class/Class.h"
namespace turbolang {
    void Class::setAllocaInst(const std::string &name, llvm::AllocaInst *allocaInst) {
        allocaMap[name] = allocaInst;
    }

    llvm::AllocaInst *Class::getAllocaInst(const std::string &name) {
        return allocaMap[name];
    }

    llvm::Value *Class::getValue(const std::string &name) {
        llvm::AllocaInst *allocaInst = getAllocaInst(name);
        llvm::LoadInst *loadInst = LLVMManager::llvmBytecodeBuilder->CreateLoad(allocaInst, name);
        return loadInst;
    }

    void Class::setValue(const std::string &name, llvm::Value *value) {
        llvm::AllocaInst *allocaInst = getAllocaInst(name);
        llvm::StoreInst *storeInst = LLVMManager::llvmBytecodeBuilder->CreateStore(value, allocaInst, false);
    }
}
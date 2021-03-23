#pragma once
#include "utils/llvm/LLVMIncluder.h"
#include "function/Function.h"
#include <string>
namespace turbolang {
    class Class {
    public:
        std::string name;
        void setAllocaInst(const std::string& name, llvm::AllocaInst* allocaInst);
        llvm::AllocaInst* getAllocaInst(const std::string& name);
        llvm::Value* getValue(const std::string& name);
        void setValue(const std::string& name, llvm::Value* value);
        std::map<std::string, Function> functions;
    private:
        std::map<std::string, llvm::AllocaInst*> allocaMap;

    };
}
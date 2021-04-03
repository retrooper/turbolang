#pragma once
#include "utils/llvm/LLVMIncluder.h"
#include "function/Function.h"
#include <string>
namespace turbolang {
    class Class {
        friend class Parser;
    public:
        std::string name;
        void setAllocaInst(const std::string& name, llvm::AllocaInst* allocaInst);
        llvm::AllocaInst* getAllocaInst(const std::string& name);
        llvm::Value* getValue(const std::string& name);
        void setValue(const std::string& name, llvm::Value* value);
        std::map<std::string, Function> functions;
        static std::map<std::string, Class> classMap;
        void create();
    private:
        llvm::StructType* structType;
        std::map<std::string, llvm::AllocaInst*> allocaMap;
    };
}
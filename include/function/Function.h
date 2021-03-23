#pragma once
#include "utils/Type.h"
#include "utils/llvm/LLVMManager.h"
#include <string>
#include <vector>
#include <map>
namespace turbolang {
    struct FunctionArgument {
        std::string name;
        DataType type;
    };

    class Function {
    public:
        static std::map<std::string, Function> functionMap;
        std::string name;
        DataType type;
        std::vector<FunctionArgument> arguments;

        llvm::Function* llvmFunction = nullptr;
        llvm::BasicBlock* entry = nullptr;
        Function() = default;
        Function(const std::string& name, DataType type);

        void create(std::vector<FunctionArgument>& inArguments);
        void create();
        void setAllocaInst(const std::string& name, llvm::AllocaInst* allocaInst);
        llvm::AllocaInst* getAllocaInst(const std::string& name);
        llvm::Value* getValue(const std::string& name);
        void setValue(const std::string& name, llvm::Value* value);
    private:

        std::map<std::string, llvm::AllocaInst*> allocaMap;
    };
}
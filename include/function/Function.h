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
        std::string extraData;
        DataType type;
        std::vector<FunctionArgument> arguments;

        llvm::Function* llvmFunction = nullptr;
        llvm::BasicBlock* entry = nullptr;
        Function() = default;
        Function(const std::string& name, DataType type, const std::string& extraData = "");

        void create();
        void setAllocaInst(const std::string& name, llvm::AllocaInst* allocaInst);
        llvm::AllocaInst* getAllocaInst(std::string name);
        llvm::Value* getValue(std::string name);
        void setValue(std::string name, llvm::Value* value);
    private:
        std::vector<std::string> split(std::string str, std::string delim);
        std::map<std::string, llvm::AllocaInst*> allocaMap;
    };
}
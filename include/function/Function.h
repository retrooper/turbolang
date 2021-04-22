#pragma once
#include "utils/Type.h"
#include "utils/llvm/LLVMManager.h"
#include <string>
#include <vector>
#include <map>
namespace turbolang {
    struct FunctionArgument {
        std::string name;
        std::string typeInfo;
        DataType type;
        bool isPtr;
    };

    class Function {
    public:
        static std::vector<Function> functions;
        std::string name;
        std::string extraData;
        DataType type;
        std::vector<FunctionArgument> arguments;

        llvm::Function* llvmFunction = nullptr;
        llvm::BasicBlock* entry = nullptr;
        Function() = default;
        Function(const std::string& name, DataType type, const std::string& extraData = "");

        static Function* getFunction(const std::string& functionName, const DataType& returnType, const std::vector<DataType>& argumentTypes);
        static std::vector<Function*> getFunctions(const std::string& functionName, const std::vector<DataType>& argumentTypes);
        static std::vector<Function*> getFunctions(const std::string& functionName);
        void create();
        void setAllocaInst(const std::string& name, llvm::AllocaInst* allocaInst);
        llvm::AllocaInst* getAllocaInst(std::string name);
        llvm::Type* getType(std::string name);
        llvm::Value* getValue(std::string name);
        llvm::Value* getDereferencedValue(std::string name);
        void setValue(std::string name, llvm::Value* value);
        void setDereferencedValue(std::string name, llvm::Value* value);
    private:
        std::vector<std::string> split(const std::string& str, const std::string& delim);
        std::map<std::string, llvm::AllocaInst*> allocaMap;
    };
}
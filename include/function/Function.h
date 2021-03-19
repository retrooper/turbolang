#pragma once
#include "variable/Variable.h"
#include "utils/Type.h"
#include "utils/llvm/LLVMManager.h"
#include <string>
#include <vector>
#include <map>
namespace turbolang {
    enum FunctionType {
        FUNCTION_TYPE_VOID = 0, FUNCTION_TYPE_BYTE = 1, FUNCTION_TYPE_SHORT = 2, FUNCTION_TYPE_INT = 3, FUNCTION_TYPE_LONG = 4,
        FUNCTION_TYPE_FLOAT = 5, FUNCTION_TYPE_DOUBLE = 6,
        FUNCTION_TYPE_BYTE_PTR = 7,  FUNCTION_TYPE_SHORT_PTR = 8,  FUNCTION_TYPE_INT_PTR = 9,  FUNCTION_TYPE_LONG_PTR = 10,
        FUNCTION_TYPE_FLOAT_PTR = 11,  FUNCTION_TYPE_DOUBLE_PTR = 12
    };
    //TODO getFunctionType(string)

    struct FunctionArgument {
        std::string name;
        VariableType type;
    };

    class Function {
    public:
        friend class Parser;
        std::string name;
        FunctionType type;
        std::vector<FunctionArgument> arguments;

        Function() = default;
        explicit Function(const std::string& name);
        Function(const std::string& name, FunctionType type);

        void create(FunctionType returnValue, std::vector<FunctionArgument>& inArguments);
        void create(FunctionType returnValue);
    private:
        llvm::Function* llvmFunction = nullptr;
        llvm::BasicBlock* entry = nullptr;
        std::map<std::string, llvm::AllocaInst*> allocaMap;
        void setAllocaInst(const std::string& name, llvm::AllocaInst* allocaInst);
        llvm::AllocaInst* getAllocaInst(const std::string& name);
        llvm::Value* getValue(const std::string& name);
        void setValue(const std::string& name, llvm::Value* value);

        static std::map<std::string, Function> functionMap;
    };
}
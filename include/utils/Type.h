#pragma once
#include "utils/llvm/LLVMManager.h"
#include <optional>
namespace turbolang {
    class Type {
    public:
        static llvm::Type* getLLVMTypeByVariableType(int type);
        static llvm::Type* getLLVMTypeByFunctionType(int type);
        static std::optional<int> getVariableType(const std::string& name);
        static std::optional<int> getFunctionType(const std::string& name);
    };
}
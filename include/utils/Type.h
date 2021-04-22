#pragma once
#include "utils/llvm/LLVMManager.h"
#include "DataType.h"
#include "class/Class.h"
#include <functional>
#include <optional>
namespace turbolang {
    class Type {
    public:
        static llvm::Type* getLLVMType(const DataType& type, const std::string& extraData = "");
        static std::optional<DataType> getType(const std::string& name);
        static std::optional<DataType> getType(const llvm::Type* type, const bool& isSigned);
        static int getBitCount(const DataType& type);
        static bool isTypeSigned(const int& type);
    };
}
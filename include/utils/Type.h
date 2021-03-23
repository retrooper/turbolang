#pragma once
#include "utils/llvm/LLVMManager.h"
#include "DataType.h"
#include <functional>
#include <optional>
namespace turbolang {
    class Type {
    public:
        static llvm::Type* getLLVMType(const int& type);
        static std::optional<DataType> getType(const std::string& name);
        static unsigned int getIntBitCount(const std::string& name);
    };
}
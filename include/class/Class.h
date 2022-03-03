#pragma once
#include "utils/llvm/LLVMIncluder.h"
#include "function/Function.h"
#include <string>
#include <map>
namespace turbolang {
    struct ClassMemberData {
        llvm::Type* type;
        bool isSigned;
        std::string name;
    };
    class Class {
        friend class Parser;
        friend class Type;
        friend class Function;
    public:
        Class() = default;
        explicit Class(const std::string& name);
        std::string name;
        static std::map<std::string, Class> classMap;
        void create();
        unsigned int getMemberIndex(const std::string& memberName);
    private:
        llvm::StructType* structType{};
        std::vector<ClassMemberData> clsMemberData;
    };
}
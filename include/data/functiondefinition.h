#pragma once

#include "type.h"
#include "parameterdefinition.h"
#include "utils/compilermanager.h"
#include <string>
#include <vector>
#include <map>

namespace turbolang {
    struct valueargumentwrapper {
        llvm::Value* value = nullptr;
        llvm::AllocaInst* allocaInst = nullptr;
        std::string name;
    };
    struct functiondefinition {
        functiondefinition() = default;

        void load(llvm::Type* returnType, std::vector<llvm::Type*>& params);

        std::string name;
        functiontype type;
        std::vector<parameterdefinition> parameters;
        std::map<std::string, llvm::Value *> variableMap;
        std::map<std::string, llvm::AllocaInst *> allocaMap;
        std::vector<turbolang::valueargumentwrapper> arguments;
        llvm::FunctionType *llvmFunctionType;
        llvm::Function *llvmFunction;
        llvm::BasicBlock *entry;

        llvm::Value* get_variable_by_name(const std::string& name);
        void set_variable_by_name(const std::string& name, llvm::Value* value);
        llvm::AllocaInst* get_alloca_instance(const std::string& name);
        void set_alloca_instance(const std::string& name, llvm::AllocaInst* allocaInst);
    };
}
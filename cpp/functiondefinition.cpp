#include "data/functiondefinition.h"

namespace turbolang {
    void functiondefinition::load(llvm::Type *returnType, std::vector<llvm::Type *> &params) {
        if (params.empty()) {
            llvmFunctionType = llvm::FunctionType::get(returnType, false);
        } else {
            llvmFunctionType = llvm::FunctionType::get(returnType, params, false);
        }
        llvmFunction = llvm::Function::Create(llvmFunctionType, llvm::Function::ExternalLinkage, name,
                                              compilermanager::llvmModule.get());
        entry = llvm::BasicBlock::Create(compilermanager::llvmContext, "entry", llvmFunction);
        std::cout << "first" << std::endl;
    }

    llvm::Value *functiondefinition::get_variable_by_name(const std::string &name) {
        for (const auto &argument : arguments) {
            if (argument.name == name) {
                return argument.value;
            }
        }
        return variableMap[name];
    }

    void functiondefinition::set_variable_by_name(const std::string &name, llvm::Value *value) {
        for (auto &argument : arguments) {
            if (argument.name == name) {
                argument.value = value;
                return;
            }
        }
        variableMap[name] = value;
    }

    llvm::AllocaInst *functiondefinition::get_alloca_instance(const std::string &name) {
        for (auto &argument : arguments) {
            if (argument.name == name) {
                llvm::Type *type = nullptr;
                llvm::Constant *size = nullptr;

                //    argument.allocaInst = turbolang::compilermanager::llvmIRBuilder.CreateAlloca(type, size, llvm::Twine(argument.name));
                return argument.allocaInst;
            }
        }
        return allocaMap[name];
    }

    void functiondefinition::set_alloca_instance(const std::string &name, llvm::AllocaInst *allocaInst) {
        for (auto &argument : arguments) {
            if (argument.name == name) {
                argument.allocaInst = allocaInst;
                return;
            }
        }
        allocaMap[name] = allocaInst;
    }
}
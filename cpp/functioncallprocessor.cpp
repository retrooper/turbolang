#include "data/functioncallprocessor.h"

namespace turbolang {

    llvm::Value* functioncallprocessor::external_func_process(turbolang::functiondefinition &targetFunction,
                                        std::vector<llvm::Value *> &arguments) {
        llvm::Function* function = turbolang::compilermanager::llvmModule->getFunction(targetFunction.name);
        return turbolang::compilermanager::llvmIRBuilder.CreateCall(function, arguments);
    }

    llvm::Value* functioncallprocessor::process(functiondefinition &currentFunction, std::vector<llvm::Value *> &arguments) {
        return nullptr;
    }

    llvm::Value* printfcallprocessor::process(turbolang::functiondefinition &currentFunction,
                                      std::vector<llvm::Value *> &arguments) {
        turbolang::printer::printf(arguments);
        return nullptr;
    }

    llvm::Value *printlncallprocessor::process(functiondefinition &currentFunction, std::vector<llvm::Value *> &arguments) {
        turbolang::printer::println(arguments);
        return nullptr;
    }

    llvm::Value* exitcallprocessor::process(functiondefinition &currentFunction, std::vector<llvm::Value*>& arguments) {
        turbolang::exiter::exit(arguments[0]);
        return nullptr;
    }
}

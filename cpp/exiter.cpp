#include "utils/exiter.h"

void turbolang::exiter::prepare_exiter() {
    std::vector<llvm::Type *> args;
    args.push_back(llvm::Type::getInt32Ty(turbolang::compilermanager::llvmContext));
    bool isFunctionArgumentSizeConstant = true;
    llvm::FunctionType *printfType =
            llvm::FunctionType::get(turbolang::compilermanager::llvmIRBuilder.getInt32Ty(), args, !isFunctionArgumentSizeConstant);
    llvm::Function::Create(printfType, llvm::Function::ExternalLinkage, "exit",
                           turbolang::compilermanager::llvmModule.get());
}

void turbolang::exiter::exit(llvm::Value* exitCodeValue) {
    std::vector<llvm::Value*> args;
    args.reserve(1);
    args.push_back(exitCodeValue);
    turbolang::compilermanager::llvmIRBuilder.CreateCall(turbolang::compilermanager::llvmModule->getFunction("exit"), args);
}
#include "utils/exiter.h"

void turbolang::exiter::prepare_exiter() {
    /*Declare that printf exists and has signature int (i8*, ...)**/
    std::vector<llvm::Type *> args;
    args.push_back(llvm::Type::getInt32Ty(turbolang::compilermanager::llvmContext));
    /*`true` specifies the function as variadic*/
    llvm::FunctionType *printfType =
            llvm::FunctionType::get(turbolang::compilermanager::llvmIRBuilder.getInt32Ty(), args, true);
    llvm::Function::Create(printfType, llvm::Function::ExternalLinkage, "exit",
                           turbolang::compilermanager::llvmModule.get());
}

void turbolang::exiter::exit(llvm::Value* exitCodeValue) {
    std::vector<llvm::Value*> args;
    args.reserve(1);
    args.push_back(exitCodeValue);
    turbolang::compilermanager::llvmIRBuilder.CreateCall(turbolang::compilermanager::llvmModule->getFunction("exit"), args);
}
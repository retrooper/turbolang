#include "utils/printer.h"

void turbolang::printer::prepare_printer(){
    /*Declare that printf exists and has signature int (i8*, ...)**/
    std::vector<llvm::Type *> args;
    args.push_back(llvm::Type::getInt8PtrTy(turbolang::compilermanager::llvmContext));
    /*`true` specifies the function as variadic*/
    llvm::FunctionType *printfType =
            llvm::FunctionType::get(turbolang::compilermanager::llvmIRBuilder.getInt32Ty(), args, true);
    llvm::Function::Create(printfType, llvm::Function::ExternalLinkage, "printf",
                           turbolang::compilermanager::llvmModule.get());
}

void turbolang::printer::printf(const std::vector<llvm::Value*>& values) {
    turbolang::compilermanager::llvmIRBuilder.CreateCall(turbolang::compilermanager::llvmModule->getFunction("printf"), values);
}
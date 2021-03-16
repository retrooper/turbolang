#include "utils/printer.h"

void turbolang::printer::prepare(){
    std::vector<llvm::Type *> args;
    args.push_back(llvm::Type::getInt8PtrTy(turbolang::compilermanager::llvmContext));
    bool doesFunctionSizeVary = false;
    llvm::FunctionType *putsType =
            llvm::FunctionType::get(turbolang::compilermanager::llvmIRBuilder.getInt32Ty(), args, doesFunctionSizeVary);
    llvm::Function::Create(putsType, llvm::Function::ExternalLinkage, "puts",
                           turbolang::compilermanager::llvmModule.get());
    doesFunctionSizeVary = true;
    llvm::FunctionType *printfType =
            llvm::FunctionType::get(turbolang::compilermanager::llvmIRBuilder.getInt32Ty(), args, doesFunctionSizeVary);
    llvm::Function::Create(printfType, llvm::Function::ExternalLinkage, "printf",
                           turbolang::compilermanager::llvmModule.get());
}

llvm::Value* turbolang::printer::printf(const std::vector<llvm::Value*>& values) {
    return turbolang::compilermanager::llvmIRBuilder.CreateCall(turbolang::compilermanager::llvmModule->getFunction("printf"), values);
}

llvm::Value* turbolang::printer::println(const std::vector<llvm::Value *> &values) {
    return turbolang::compilermanager::llvmIRBuilder.CreateCall(turbolang::compilermanager::llvmModule->getFunction("puts"), values);
}

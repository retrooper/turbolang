#include "utils/scanner.h"
namespace turbolang {

    void scanner::prepare() {
        std::vector<llvm::Type *> args;
        args.push_back(llvm::Type::getInt8PtrTy(turbolang::compilermanager::llvmContext));
        bool doesFunctionSizeVary = true;
        llvm::FunctionType *scanfType =
                llvm::FunctionType::get(turbolang::compilermanager::llvmIRBuilder.getInt32Ty(), args, doesFunctionSizeVary);
        llvm::Function::Create(scanfType, llvm::Function::ExternalLinkage, "scanf",
                               turbolang::compilermanager::llvmModule.get());
    }

    llvm::Value* scanner::scanf(const std::vector<llvm::Value *> &values) {
        return compilermanager::llvmIRBuilder.CreateCall(compilermanager::llvmModule->getFunction("scanf"), values);
    }
}

